use std::any::Any;
use std::cell::RefCell;
use std::collections::HashMap;
use std::f32::consts::PI;
use std::future::{Future, IntoFuture};
use std::io::{BufRead, BufReader};
use std::rc::Rc;

use gl_matrix::{mat4, vec3};
use gl_matrix::common::Mat4;
use js_sys::{Float32Array, JsString};
use wasm_bindgen::prelude::*;
use wasm_bindgen_futures::{JsFuture, spawn_local};
use wavefront_obj::obj::{ObjSet, Primitive};
use wavefront_obj::ParseError;
use web_sys::{Event, KeyboardEvent, MouseEvent, Request, RequestInit, RequestMode, Text, WebGl2RenderingContext, WebGlBuffer, WebGlFramebuffer, WebGlRenderbuffer, WebGlTexture, WebGlUniformLocation, window};
use web_sys::console::log_1;

use crate::webgl_link::{compile_shader, create_array_buffer_view, link_program};

mod shape;
mod color;
mod webgl_link;

#[derive(Default, Debug)]
struct Query1 {
    pub offset_x: f32,
    pub offset_y: f32,
    pub degree: [f32; 3],
    pub pressed: bool,
    pub zoom: f32,
}

#[derive(Default, Debug)]
struct Query2 {
    pub offset_x: f32,
    pub offset_y: f32,
    pub offset_z: f32,
    pub dragged: bool,
    pub degree: [f32; 2]
}

#[derive(Default, Debug)]
struct QueryEnvironment {
    pub last_x: f64,
    pub last_y: f64,
    pub rotate_x: f32,
    pub rotate_y: f32,
    pub dragged: bool
}
#[derive(Debug)]
struct ProgramParameter {
    pub u_mvp_matrix: WebGlUniformLocation,
    pub u_model_matrix: WebGlUniformLocation,
    pub u_normal_matrix: WebGlUniformLocation,
    pub u_light_position: WebGlUniformLocation,
    pub u_view_position: WebGlUniformLocation,
    pub u_ka: WebGlUniformLocation,
    pub u_kd: WebGlUniformLocation,
    pub u_ks: WebGlUniformLocation,
    pub u_shininess: WebGlUniformLocation,
    pub a_color: i32,
    pub a_position: i32,
    pub a_normal: i32,
    pub color_buffer: WebGlBuffer,
    pub position_buffer: WebGlBuffer,
    pub normal_buffer: WebGlBuffer,
    pub texture: WebGlTexture,
    pub frame_buffer: WebGlFramebuffer,
    pub depth_buffer: WebGlRenderbuffer,
    pub u_shadowmap: WebGlUniformLocation,
    pub quad_a_position: i32,
    pub quad_u_mvp_matrix: WebGlUniformLocation,
}

fn in_circle(radius: f32, circle: &[f32; 3], point: &[f32; 3]) -> bool {
    let x_diff = circle[0] - point[0];
    let y_diff = circle[1] - point[1];
    let z_diff = circle[2] - point[2];

    if x_diff * x_diff + y_diff * y_diff + z_diff * z_diff <= radius * radius * radius {
        return true;
    }

    return false;
}


pub async fn new_obj_with_url(url: &str) -> Result<ObjSet, ParseError> {
    let text = get_text(&url).await;
    let text = text.unwrap();
    let v = wavefront_obj::obj::parse(&text);
    v
}

async fn get_text(url: &str) -> Result<String, JsValue> {
    let mut options = RequestInit::default();
    options.method("GET");
    options.mode(RequestMode::Cors);
    let request = Request::new_with_str_and_init(url, &options).unwrap();
    let response = JsFuture::from(window().unwrap().fetch_with_request(&request)).await;
    let response = response.unwrap();
    let response: web_sys::Response = response.dyn_into().unwrap();
    let text = JsFuture::from(response.text().unwrap()).await;
    let text = text.unwrap();
    Ok(text.as_string().unwrap())
}

const QUAD_VSHADER: &str= r##"
    attribute vec4 a_Position;
    uniform mat4 u_MvpMatrix;
    varying vec4 v_Position;
    void main(){
        gl_Position = u_MvpMatrix * a_Position;
        v_Position = a_Position;
    }
"##;

const QUAD_FSHADER: &str = r##"
    precision mediump float;
    varying vec4 v_Position;
    uniform sampler2D u_ShadowMap;
    void main(){
      //TODO-2: look up the depth from u_ShaodowMap and draw on quad (just one line)
      vec4 depth = texture2D(u_ShadowMap, v_Position.xy / 2.0 + 0.5);
      gl_FragColor = vec4(depth.r, depth.g, depth.b, depth.a);
    }
"##;

const GENERIC_VSHADER: &str = r##"
        attribute vec4 a_Position;
        attribute vec4 a_Color;
        attribute vec4 a_Normal;
        uniform mat4 u_MvpMatrix;
        uniform mat4 u_modelMatrix;
        uniform mat4 u_normalMatrix;
        varying vec3 v_Normal;
        varying vec3 v_PositionInWorld;
        varying vec4 v_Color;
        void main(){
            gl_Position = u_MvpMatrix * a_Position;
            v_PositionInWorld = (u_modelMatrix * a_Position).xyz;
            v_Normal = normalize((u_normalMatrix * a_Normal).xyz);
            v_Color = a_Color;
            //TODO-3: transform normal vector "a_Normal" to world space using "u_normalMatrix" and store the result in "v_Normal",
            //        remember to renormalize the result before storing it to v_Normal
            //TODO-4: set "a_Color" to "v_Color"
        }
        "##;

const GENERAL_FSHADER: &str = r##"
            precision mediump float;
            uniform vec3 u_LightPosition;
            uniform vec3 u_ViewPosition;
            uniform float u_Ka;
            uniform float u_Kd;
            uniform float u_Ks;
            uniform float u_shininess;
            varying vec3 v_Normal;
            varying vec3 v_PositionInWorld;
            varying vec4 v_Color;
            void main(){
                // let ambient and diffuse color are v_Color
                // (you can also input them from ouside and make them different)
                vec3 ambientLightColor = v_Color.rgb;
                vec3 diffuseLightColor = v_Color.rgb;
                // assume white specular light (you can also input it from ouside)
                vec3 specularLightColor = vec3(1.0, 1.0, 1.0);

                //TODO-5: calculate ambient light color using "ambientLightColor" and "u_Ka"
                vec3 ambient = ambientLightColor * u_Ka;

                vec3 normal = normalize(v_Normal); //normalize the v_Normal before using it, before it comes from normal vectors interpolation
                //TODO-6: calculate diffuse light color using "normal", "u_LightPosition", "v_PositionInWorld", "diffuseLightColor", and "u_Kd"
                vec3 lightDirection = normalize(u_LightPosition - v_PositionInWorld);
                float nDotL = max(dot(lightDirection, normal), 0.0);
                vec3 diffuse = diffuseLightColor * u_Kd * nDotL;

                vec3 specular = vec3(0.0, 0.0, 0.0);
                if(nDotL > 0.0) {
                    //TODO-7: calculate specular light color using "normal", "u_LightPosition", "v_PositionInWorld",
                    //       "u_ViewPosition", "u_shininess", "specularLightColor", and "u_Ks"
                    //   You probably can store the result of specular calculation into "specular" variable
                    vec3 R = reflect(-lightDirection, normal);
                    // V: the vector, point to viewer
                    vec3 V = normalize(u_ViewPosition - v_PositionInWorld);
                    float specAngle = clamp(dot(R, V), 0.0, 1.0);
                    specular = u_Ks * pow(specAngle, u_shininess) * specularLightColor;
                }

                //TODO-8: sum up ambient, diffuse, specular light color from above calculation and put them into "gl_FragColor"
                gl_FragColor = vec4( ambient + diffuse + specular, 1.0 );
            }
            "##;


#[wasm_bindgen(start)]
async fn start() -> Result<(), JsValue> {
    let document = window().unwrap().document().unwrap();
    let canvas = document.get_element_by_id("canvas").unwrap();
    let canvas: web_sys::HtmlCanvasElement = canvas.dyn_into::<web_sys::HtmlCanvasElement>()?;

    let context = canvas
        .get_context("webgl2")?
        .unwrap()
        .dyn_into::<WebGl2RenderingContext>()?;

    let context2 = context.clone();

    let vert_shader = compile_shader(
        &context,
        WebGl2RenderingContext::VERTEX_SHADER,
        GENERIC_VSHADER,
    )?;

    let frag_shader = compile_shader(
        &context,
        WebGl2RenderingContext::FRAGMENT_SHADER,
        GENERAL_FSHADER,
    )?;

    let quad_vert_shader = compile_shader(
        &context,
        WebGl2RenderingContext::VERTEX_SHADER,
        QUAD_VSHADER,
    )?;

    let quad_frag_shader = compile_shader(
        &context,
        WebGl2RenderingContext::FRAGMENT_SHADER,
        QUAD_FSHADER,
    )?;


    let program = link_program(&context, &vert_shader, &frag_shader)?;
    let quad_program = link_program(&context, &quad_vert_shader, &quad_frag_shader)?;
    context.use_program(Some(&program));


    let q1 = Rc::new(RefCell::new(Query1::default()));
    let q2 = Rc::new(RefCell::new(Query2::default()));
    let qe = Rc::new(RefCell::new(QueryEnvironment::default()));

    {
        let mut q2 = q2.borrow_mut();
        q2.offset_x = 0.0;
        q2.offset_y = 0.9;
        q2.offset_z = 0.5;
    }

    let quad = [
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        -0.5,  0.5, 0.0,
        -0.5,  0.5, 0.0,
        0.5, -0.5, 0.0,
        0.5,  0.5, 0.0
    ];

    let mut envir = Rc::new(RefCell::new(ProgramParameter {
        u_mvp_matrix: context.get_uniform_location(&program, "u_MvpMatrix").unwrap(),
        u_model_matrix: context.get_uniform_location(&program, "u_modelMatrix").unwrap(),
        u_normal_matrix: context.get_uniform_location(&program, "u_normalMatrix").unwrap(),
        u_light_position: context.get_uniform_location(&program, "u_LightPosition").unwrap(),
        u_view_position: context.get_uniform_location(&program, "u_ViewPosition").unwrap(),
        u_ka: context.get_uniform_location(&program, "u_Ka").unwrap(),
        u_kd: context.get_uniform_location(&program, "u_Kd").unwrap(),
        u_ks: context.get_uniform_location(&program, "u_Ks").unwrap(),
        u_shininess: context.get_uniform_location(&program, "u_shininess").unwrap(),
        a_color: context.get_attrib_location(&program, "a_Color"),
        a_position: context.get_attrib_location(&program, "a_Position"),
        a_normal: context.get_attrib_location(&program, "a_Normal"),
        color_buffer: context.create_buffer().unwrap(),
        position_buffer: context.create_buffer().unwrap(),
        normal_buffer: context.create_buffer().unwrap(),
        texture: context.create_texture().unwrap(),
        frame_buffer: context.create_framebuffer().unwrap(),
        depth_buffer: context.create_renderbuffer().unwrap(),
        u_shadowmap: context.get_uniform_location(&quad_program, "u_ShadowMap").unwrap(),
        quad_a_position: context.get_attrib_location(&quad_program, "a_Position"),
        quad_u_mvp_matrix: context.get_uniform_location(&quad_program, "u_MvpMatrix").unwrap()
    }));

    {
        let mut envir = envir.clone();
        let mut envir = envir.borrow_mut();
        if envir.a_color == -1 {
            log_1(&JsString::from("GG"));
        }

        context.bind_texture(WebGl2RenderingContext::TEXTURE_2D, Some(&envir.texture));
        context.tex_image_2d_with_i32_and_i32_and_i32_and_format_and_type_and_opt_array_buffer_view(WebGl2RenderingContext::TEXTURE_2D
                                                                                  , 0
                                                                                  , WebGl2RenderingContext::RGBA as i32
                                                                                  , 2048
                                                                                  , 2048
                                                                                  , 0
                                                                                  , WebGl2RenderingContext::RGBA
                                                                                  , WebGl2RenderingContext::UNSIGNED_BYTE, None).unwrap();
        context.tex_parameteri(WebGl2RenderingContext::TEXTURE_2D, WebGl2RenderingContext::TEXTURE_MIN_FILTER, WebGl2RenderingContext::LINEAR as i32);
        context.pixel_storei(WebGl2RenderingContext::UNPACK_FLIP_Y_WEBGL, 1);

        context.bind_renderbuffer(WebGl2RenderingContext::RENDERBUFFER, Some(&envir.depth_buffer));
        context.renderbuffer_storage(WebGl2RenderingContext::RENDERBUFFER, WebGl2RenderingContext::DEPTH_COMPONENT16, 2048, 2048);

        context.bind_framebuffer(WebGl2RenderingContext::FRAMEBUFFER, Some(&envir.frame_buffer));
        context.framebuffer_texture_2d(WebGl2RenderingContext::FRAMEBUFFER, WebGl2RenderingContext::COLOR_ATTACHMENT0, WebGl2RenderingContext::TEXTURE_2D, Some(&envir.texture), 0);
        context.framebuffer_renderbuffer(WebGl2RenderingContext::FRAMEBUFFER, WebGl2RenderingContext::DEPTH_ATTACHMENT, WebGl2RenderingContext::RENDERBUFFER, Some(&envir.depth_buffer));
    }

    context.enable(WebGl2RenderingContext::DEPTH_TEST);

    let mut sword = Rc::new(RefCell::new(new_obj_with_url("assets/sword.obj").await.unwrap()));
    let mut order = HashMap::new();
    order.insert("Lame_Plane".to_string(), 0);
    order.insert("Garde_Plane.002".to_string(), 1);
    order.insert("Pommeau_Plane.001".to_string(), 3);
    order.insert("Manche_Circle".to_string(), 2);
    {
        let mut sword = sword.borrow_mut();
        sword.objects.sort_by(|lhs, rhs| {
            order.get(&lhs.name).unwrap().partial_cmp(order.get(&rhs.name).unwrap()).unwrap()
        });
    }

    let draw = {
        let mut q1 = q1.clone();
        let mut q2 = q2.clone();
        let mut qe = qe.clone();
        let sword = sword.clone();
        let envir = envir.clone();

        move || {
            let mut q1 = q1.borrow_mut();
            let mut q2 = q2.borrow_mut();
            let qe = qe.borrow();
            let sword = sword.borrow();
            let envir = envir.borrow();

            let vertices = create_array_buffer_view(&[
                1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, //front
                1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, //right
                1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, //up
                -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, //left
                -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0,  1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, //bottom
                1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0 //back
            ]);

            let red = create_array_buffer_view(&[
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0
            ]);

            let color = create_array_buffer_view(&[
                0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, //front
                0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, //right
                1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, //up
                1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, //left
                1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, //bottom
                0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, //back
            ]);

            let green = create_array_buffer_view(&[
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
            ]);

            let blue = create_array_buffer_view(&[
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
            ]);

            let normals = create_array_buffer_view(&[
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, //front
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, //right
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, //up
                -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, //left
                0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, //bottom
                0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0 //back
            ]);

            context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.position_buffer));
            context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &vertices, WebGl2RenderingContext::STATIC_DRAW);
            context.vertex_attrib_pointer_with_i32(envir.a_position as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
            context.enable_vertex_attrib_array(envir.a_position as u32);

            context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.color_buffer));
            context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &color, WebGl2RenderingContext::STATIC_DRAW);
            context.vertex_attrib_pointer_with_i32(envir.a_color as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
            context.enable_vertex_attrib_array(envir.a_color as u32);

            context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.normal_buffer));
            context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &normals, WebGl2RenderingContext::STATIC_DRAW);
            context.vertex_attrib_pointer_with_i32(envir.a_normal as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
            context.enable_vertex_attrib_array(envir.a_normal as u32);

            context.uniform3f(Some(&envir.u_light_position), 1.0, 1.0, 3.0);
            context.uniform3f(Some(&envir.u_view_position), 0.0, 3.0 + q1.zoom, 4.0 + q1.zoom);
            context.uniform1f(Some(&envir.u_ka), 0.2);
            context.uniform1f(Some(&envir.u_kd), 0.7);
            context.uniform1f(Some(&envir.u_ks), 1.0);
            context.uniform1f(Some(&envir.u_shininess), 10.0);

            let mut stk = vec![];

            let mut model = mat4::create();
            let tmp = model.clone();
            mat4::rotate_x(&mut model, &tmp, qe.rotate_x);
            let tmp = model.clone();
            mat4::rotate_y(&mut model, &tmp, qe.rotate_y);
            stk.push(model);

            let mut mv = mat4::create();
            mat4::perspective(&mut mv, 30.0, 1.0, 1.0, Some(100.0));
            let mut lookat = mat4::create();
            mat4::look_at(&mut lookat, &vec3::from_values(0.0, 3.0 + q1.zoom, 4.0 + q1.zoom), &vec3::from_values(0.0, 0.0, 0.0), &vec3::from_values(0.0, -1.0, 0.0));
            let tmp1 = mv.clone();
            mat4::multiply(&mut mv, &tmp1, &lookat);

            let draw_cube = |model: Mat4, mut mvp: Mat4, color: Float32Array| {
                let tmp1 = mvp.clone();
                let tmp2 = model.clone();
                mat4::multiply(&mut mvp, &tmp1, &tmp2);

                let mut normal = mat4::create();
                mat4::invert(&mut normal, &model).unwrap();
                let tmp = normal.clone();
                mat4::transpose(&mut normal, &tmp);

                // Bind color
                context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.color_buffer));
                context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &color, WebGl2RenderingContext::STATIC_DRAW);
                context.vertex_attrib_pointer_with_i32(envir.a_color as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
                context.enable_vertex_attrib_array(envir.a_color as u32);

                // Bind verties
                context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.position_buffer));
                context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &vertices, WebGl2RenderingContext::STATIC_DRAW);
                context.vertex_attrib_pointer_with_i32(envir.a_position as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
                context.enable_vertex_attrib_array(envir.a_position as u32);

                // Bind normal
                context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.normal_buffer));
                context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &normals, WebGl2RenderingContext::STATIC_DRAW);
                context.vertex_attrib_pointer_with_i32(envir.a_normal as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
                context.enable_vertex_attrib_array(envir.a_normal as u32);

                context.uniform_matrix4fv_with_f32_array(Some(&envir.u_mvp_matrix), false, &mvp);
                context.uniform_matrix4fv_with_f32_array(Some(&envir.u_model_matrix), false, &model);
                context.uniform_matrix4fv_with_f32_array(Some(&envir.u_normal_matrix), false, &normal);

                context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, 36);
            };

            let draw_triangle = |model: Mat4, mut mvp: Mat4, color: Float32Array, verts: &[f32], norm: &[f32]| {
                // log_1(&JsString::from(format!("{:?}", norm)));
                let tmp1 = mvp.clone();
                let tmp2 = model.clone();
                mat4::multiply(&mut mvp, &tmp1, &tmp2);

                let mut normal = mat4::create();
                mat4::invert(&mut normal, &model).unwrap();
                let tmp = normal.clone();
                mat4::transpose(&mut normal, &tmp);

                // Bind color
                context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.color_buffer));
                context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &color, WebGl2RenderingContext::STATIC_DRAW);
                context.vertex_attrib_pointer_with_i32(envir.a_color as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
                context.enable_vertex_attrib_array(envir.a_color as u32);
                context.vertex_attrib_pointer_with_i32(envir.a_position as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);

                // Bind verties
                context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.position_buffer));
                let vertices = create_array_buffer_view(verts);
                context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &vertices, WebGl2RenderingContext::STATIC_DRAW);
                context.vertex_attrib_pointer_with_i32(envir.a_position as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
                context.enable_vertex_attrib_array(envir.a_position as u32);

                // Bind Normal
                context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.normal_buffer));
                let normals = create_array_buffer_view(norm);
                context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &normals, WebGl2RenderingContext::STATIC_DRAW);
                context.vertex_attrib_pointer_with_i32(envir.a_normal as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
                context.enable_vertex_attrib_array(envir.a_normal as u32);

                context.uniform_matrix4fv_with_f32_array(Some(&envir.u_mvp_matrix), false, &mvp);
                context.uniform_matrix4fv_with_f32_array(Some(&envir.u_model_matrix), false, &model);
                context.uniform_matrix4fv_with_f32_array(Some(&envir.u_normal_matrix), false, &normal);

                context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, 3);
            };


            context.clear_color(0.0, 0.0, 0.0, 1.0);
            context.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT | WebGl2RenderingContext::DEPTH_BUFFER_BIT);

            // Base
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(0.0, 0.0, -1.0));
            let tmp = model.clone();
            mat4::scale(&mut model, &tmp, &vec3::from_values(2.5, 0.5, 2.5));
            draw_cube(model.clone(), mv.clone(), color.clone());

            // Light
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(1.0, 1.0, 3.0));
            let tmp = model.clone();
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.5, 0.5, 0.5));
            draw_cube(model.clone(), mv.clone(), red.clone());

            // Robot
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(q1.offset_x, 0.0, q1.offset_y));
            stk.push(model);


            // Body
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(0.0, 0.8, -1.0));
            stk.push(model.clone());
            let tmp = model.clone();
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.3, 0.2, 0.3));
            draw_cube(model.clone(), mv.clone(), red.clone());

            // Leg1
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.15, -0.25, 0.0));
            let tmp = model.clone();
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.1, 0.1, 0.1));
            draw_cube(model.clone(), mv.clone(), green.clone());

            // Leg2
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(0.15, -0.25, 0.0));
            let tmp = model.clone();
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.1, 0.1, 0.1));
            draw_cube(model.clone(), mv.clone(), green.clone());

            // Head
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(0.0, 0.3, 0.0));
            let tmp = model.clone();
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.15, 0.12, 0.15));
            draw_cube(model.clone(), mv.clone(), blue.clone());

            // Articulate1
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.4, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.05, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), blue.clone());

            // Hand1
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::rotate_z(&mut model, &tmp, q1.degree[0] / 360.0);
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.2, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.1, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), green.clone());

            // Articulate2
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.2, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.05, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), blue.clone());

            // Hand2
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::rotate_z(&mut model, &tmp, q1.degree[1] / 360.0);
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.2, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.1, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), green.clone());

            // Articulate3
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.2, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.05, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), blue.clone());

            // Hand3
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::rotate_z(&mut model, &tmp, q1.degree[2] / 360.0);
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.2, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.1, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), green.clone());

            let judge_mat = stk.last().unwrap().clone();

            stk.pop();
            stk.pop();
            stk.pop();
            stk.pop();
            stk.pop();
            stk.pop();

            // Rhand, useless
            // Articulate1
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(0.4, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.05, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), blue.clone());

            // Hand1
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(0.2, 0.0, 0.0));
            let tmp = model.clone();
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.1, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), green.clone());
            stk.pop();
            stk.pop();
            stk.pop();

            // Judge if held?
            let mut v3 = vec3::create();
            let tmp = v3.clone();
            vec3::transform_mat4(&mut v3, &tmp, &judge_mat);

            // log_1(&JsString::from(format!("{:?} {:?}", v3, q2)));

            if q1.pressed {
                q1.pressed = false;
                if in_circle(0.5, &v3, &[q2.offset_x, q2.offset_y, q2.offset_z]) {
                    q2.dragged = !q2.dragged;
                }
            }

            let mut color = green.clone();

            if q2.dragged {
                q2.offset_x = v3[0];
                q2.offset_y = v3[1];
                q2.offset_z = v3[2];
                color = red.clone();
            }

            let mut model = mat4::create();
            let tmp = model.clone();
            mat4::rotate_x(&mut model, &tmp, qe.rotate_x);
            let tmp = model.clone();
            mat4::rotate_y(&mut model, &tmp, qe.rotate_y);
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(q2.offset_x, q2.offset_y, q2.offset_z));
            stk.push(model);

            // Hand1
            let mut model = stk.last().unwrap().clone();
            stk.push(model.clone());
            let tmp = model.clone();
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.1, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), color.clone());

            // Articulate2
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.2, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.05, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), blue.clone());

            // Hand2
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::rotate_z(&mut model, &tmp, q2.degree[0] / 360.0);
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.2, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.1, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), color.clone());

            // Articulate3
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.2, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.05, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), blue.clone());

            // Hand3
            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::rotate_z(&mut model, &tmp, q2.degree[1] / 360.0);
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(-0.2, 0.0, 0.0));
            let tmp = model.clone();
            stk.push(model.clone());
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.1, 0.05, 0.05));
            draw_cube(model.clone(), mv.clone(), color.clone());

            let mut model = stk.last().unwrap().clone();
            let tmp = model.clone();
            mat4::scale(&mut model, &tmp, &vec3::from_values(0.03, 0.03, 0.03));

            // Draw Sword
            for obj in &sword.objects {
                for geo in &obj.geometry {
                    for shape in &geo.shapes {
                        match shape.primitive {
                            Primitive::Triangle(a, b, c) => {
                                let (a_vert, _, a_norm) = a;
                                let (b_vert, _, b_norm) = b;
                                let (c_vert, _, c_norm) = c;
                                let a_norm = a_norm.unwrap();
                                let b_norm = b_norm.unwrap();
                                let c_norm = c_norm.unwrap();
                                draw_triangle(model.clone(), mv.clone(), if q2.dragged {green.clone()} else {red.clone()}, &[
                                    obj.vertices[a_vert].x as f32, obj.vertices[a_vert].y as f32, obj.vertices[a_vert].z as f32,
                                    obj.vertices[b_vert].x as f32, obj.vertices[b_vert].y as f32, obj.vertices[b_vert].z as f32,
                                    obj.vertices[c_vert].x as f32, obj.vertices[c_vert].y as f32, obj.vertices[c_vert].z as f32], &[
                                    obj.normals[a_norm].x as f32, obj.normals[a_norm].y as f32, obj.normals[a_norm].z as f32,
                                    obj.normals[b_norm].x as f32, obj.normals[b_norm].y as f32, obj.normals[b_norm].z as f32,
                                    obj.normals[c_norm].x as f32, obj.normals[c_norm].y as f32, obj.normals[c_norm].z as f32]);
                            }
                            _ => {

                            }
                        }
                    }
                }
            }
        }
    };

    let draw_all = {
        let envir = envir.clone();
        let qe = qe.clone();
        let q1 = q1.clone();

        move || {
            context2.use_program(Some(&program));
            let envir = envir.borrow();
            context2.bind_framebuffer(WebGl2RenderingContext::FRAMEBUFFER, Some(&envir.frame_buffer));
            context2.viewport(0, 0, 2048, 2048);
            context2.clear_color(0.0, 0.0, 0.0, 1.0);
            context2.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT | WebGl2RenderingContext::DEPTH_BUFFER_BIT);
            context2.enable(WebGl2RenderingContext::DEPTH_TEST);
            draw();
            context2.bind_framebuffer(WebGl2RenderingContext::FRAMEBUFFER, None);
            context2.viewport(0, 0, 800, 800);
            context2.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT | WebGl2RenderingContext::DEPTH_BUFFER_BIT);
            context2.enable(WebGl2RenderingContext::DEPTH_TEST);
            draw();

            context2.use_program(Some(&quad_program));
            context2.active_texture(WebGl2RenderingContext::TEXTURE0);
            context2.bind_texture(WebGl2RenderingContext::TEXTURE_2D, Some(&envir.texture));

            let qe = qe.borrow();
            let q1 = q1.borrow();
            context2.uniform1i(Some(&envir.u_shadowmap), 0);
            context2.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&envir.position_buffer));
            let vertices = create_array_buffer_view(&quad);
            context2.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &vertices, WebGl2RenderingContext::STATIC_DRAW);
            context2.vertex_attrib_pointer_with_i32(envir.quad_a_position as u32, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
            context2.enable_vertex_attrib_array(envir.quad_a_position as u32);

            let mut model = mat4::create();
            let tmp = model.clone();
            mat4::rotate_x(&mut model, &tmp, qe.rotate_x);
            let tmp = model.clone();
            mat4::rotate_y(&mut model, &tmp, qe.rotate_y);
            let tmp = model.clone();
            mat4::translate(&mut model, &tmp, &vec3::from_values(2.0, 2.0, 2.0));
            // let tmp = model.clone();
            // mat4::scale(&mut model, &tmp, &vec3::from_values(5.0, 10.0, 10.0));
            let mut vp = mat4::create();
            mat4::perspective(&mut vp, 30.0, 1.0, 1.0, Some(100.0));
            let mut lookat = mat4::create();
            mat4::look_at(&mut lookat, &vec3::from_values(0.0, 3.0 + q1.zoom, 4.0 + q1.zoom), &vec3::from_values(0.0, 0.0, 0.0), &vec3::from_values(0.0, -1.0, 0.0));
            let tmp1 = vp.clone();
            mat4::multiply(&mut vp, &tmp1, &lookat);
            let tmp1 = vp.clone();
            mat4::multiply(&mut vp, &tmp1, &model);

            context2.uniform_matrix4fv_with_f32_array(Some(&envir.quad_u_mvp_matrix), false, &vp);
            context2.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, 6);
        }
    };

    {
        let q1 = q1.clone();
        let q2 = q2.clone();
        let draw = draw_all.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |keyboard_event: KeyboardEvent| {
            let mut key = keyboard_event.key();
            key = key.to_uppercase();

            let mut q1 = q1.borrow_mut();
            let mut q2 = q2.borrow_mut();

            match &key[..] {
                "A" => q1.offset_x -= 0.05,
                "D" => q1.offset_x += 0.05,
                "S" => q1.offset_y -= 0.05,
                "W" => q1.offset_y += 0.05,
                "I" => q1.degree[0] += 10.0,
                "O" => q1.degree[0] -= 10.0,
                "K" => q1.degree[1] += 10.0,
                "L" => q1.degree[1] -= 10.0,
                "N" => q1.degree[2] += 10.0,
                "M" => q1.degree[2] -= 10.0,
                "Y" => q2.degree[0] += 10.0,
                "U" => q2.degree[0] -= 10.0,
                "H" => q2.degree[1] += 10.0,
                "J" => q2.degree[1] -= 10.0,
                "R" => q1.pressed = true,
                _ => {}
            }

            drop(q2);
            drop(q1);

            draw();
        });

        document.add_event_listener_with_callback("keydown", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }

    {
        let qe = qe.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |mouse_event: MouseEvent| {
            let mut qe = qe.borrow_mut();
            let (x, y) = (mouse_event.client_x() as f64, mouse_event.client_y() as f64);

            let rect = mouse_event.target().unwrap().dyn_into::<web_sys::HtmlElement>().unwrap().get_bounding_client_rect();
            if (0.0..800.0).contains(&x) && (0.0..800.0).contains(&y) {
                qe.last_x = x;
                qe.last_y = y;
                qe.dragged = true;
            }
        });

        document.add_event_listener_with_callback("mousedown", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }

    {
        let qe = qe.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |mouse_event: MouseEvent| {
            let mut qe = qe.borrow_mut();
            qe.dragged = false;
        });

        document.add_event_listener_with_callback("mouseup", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }

    {
        let qe = qe.clone();
        let draw = draw_all.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |mouse_event: MouseEvent| {
            let mut qe = qe.borrow_mut();
            let (x, y) = (mouse_event.client_x() as f64, mouse_event.client_y() as f64);

            if qe.dragged {
                let factor = (100.0 / 800.0);
                let (dx, dy) = (factor * (x - qe.last_x), factor * (y - qe.last_y));
                qe.rotate_x += dy as f32;
                qe.rotate_y += dx as f32;
            }

            qe.last_x = x;
            qe.last_y = y;

            drop(qe);
            draw();
        });

        document.add_event_listener_with_callback("mousemove", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }


    {
        let q1 = q1.clone();
        let doc = document.clone();
        let closure = Closure::<dyn Fn(_)>::new(move |e: Event| {
            let mut q1 = q1.borrow_mut();
            let input = e
                .current_target()
                .unwrap()
                .dyn_into::<web_sys::HtmlInputElement>()
                .unwrap();
            q1.zoom = -(input.value().parse::<f32>().unwrap() / 100.0);
        });
        document.get_element_by_id("abc").unwrap().add_event_listener_with_callback("input", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }

    draw_all();

    Ok(())
}
