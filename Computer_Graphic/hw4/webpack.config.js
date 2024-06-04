const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const webpack = require('webpack');
const WasmPackPlugin = require("@wasm-tool/wasm-pack-plugin");
const CopyPlugin = require("copy-webpack-plugin");


module.exports = {
    entry: './index.js',
    output: {
        path: path.resolve(__dirname, 'dist'),
        filename: 'index.js',
    },
    devServer: {
        static: [{
            directory: path.resolve(__dirname, "assets"),
            publicPath: "/assets"
        }]
    },
    plugins: [
        new HtmlWebpackPlugin({
            template: 'index.html'
        }),
        new WasmPackPlugin({
            crateDirectory: path.resolve(__dirname, ".")
        }),
        // Have this example work in Edge which doesn't ship `TextEncoder` or
        // `TextDecoder` at this time.
        new webpack.ProvidePlugin({
            TextDecoder: ['text-encoding', 'TextDecoder'],
            TextEncoder: ['text-encoding', 'TextEncoder']
        }),
        new CopyPlugin({
            patterns: [
                { from: path.resolve(__dirname, 'src/assets'), to: 'assets' } // to the dist root directory
            ],
        }),
    ],
    mode: 'development',
    experiments: {
        asyncWebAssembly: true
    }
};
