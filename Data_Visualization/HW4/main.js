const MARGIN = {LEFT: 10, RIGHT: 10, TOP: 10, DOWN: 10};
const WIDTH = 1200 - MARGIN.LEFT - MARGIN.RIGHT;
const HEIGHT = 800 - MARGIN.TOP - MARGIN.DOWN;
const colorMapper = {"ptsNorm": "red", "rebNorm": "green", "astNorm": "blue"};
const locMap = {};
const barChartsMapper = {};
const showMap = {};
let playerData = [];
let origPlayerData;
let onClickPlayerDataGraph = true;
let dotsInAmerica;
let currentBars = {};
let dots;
let showCnt = 0;


const chart = d3.select("#chart-region").append("svg")
    .attr("width", WIDTH + MARGIN.LEFT + MARGIN.RIGHT)
    .attr("height", HEIGHT + MARGIN.TOP + MARGIN.DOWN)
    .style("border", "1px solid #00000060");


const tips = d3.tip()
    .attr('class', 'd3-tip')
    .html(d => ("Name: " + d.player_name + "</br>pts: " + d.pts + "</br>reb: " + d.reb + "</br>ast: " + d.ast));

const barCharts = chart.append("g");
const playerChart = chart.append("g");

async function playerDataExtractor(csvData) {
    csvData.forEach( data => {
        data.age = Number(data.age)
        data.draft_number = data.draft_number === "Undrafted" ? 61 : Number(data.draft_number);
        data.gp = Number(data.gp);
        data.pts = Number(data.pts);
        data.reb = Number(data.reb);
        data.ast = Number(data.ast);
        data.umapX = Number(data.umapX);
        data.umapY = Number(data.umapY);
        data.ptsNorm = Number(data.ptsNorm);
        data.rebNorm = Number(data.rebNorm);
        data.astNorm = Number(data.astNorm);
        locMap[data.team_abbreviation].tot++
    });

    return csvData;
}

function getMax(data) {
    if(data.ptsNorm > data.rebNorm && data.ptsNorm > data.astNorm) {
        return "ptsNorm";
    } else if (data.rebNorm > data.ptsNorm && data.rebNorm > data.astNorm) {
        return "rebNorm";
    } else {
        return "astNorm";
    }
}



async function scatterPlotHandler(data) {
    const W = WIDTH * 0.4;
    const H = HEIGHT * 0.5;
    const X = MARGIN.LEFT;
    const Y = MARGIN.TOP;
    const playerGraph = playerChart.attr("width", W).attr("height", H);
    const xTicks = d3.scaleLinear().domain(d3.extent(data.map(d => d.umapX))).range([0, W]);
    const yTicks = d3.scaleLinear().domain(d3.extent(data.map(d => d.umapY))).range([H, 0]);
    const sizeTicks = d3.scaleLinear().domain(d3.extent(data.map(d => d.ptsNorm))).range([2, 7]);
    origPlayerData = data;

    // x label
    playerGraph.append("g")
        .attr("transform", `translate(${X},  ${Y + H})`)
        .call(d3.axisBottom(xTicks))
        .selectAll("text")
        .style("display", "none")

    // y label
    playerGraph.append("g")
        .attr("transform", `translate(${X},  ${Y})`)
        .call(d3.axisLeft(yTicks))
        .selectAll("text")
        .style("display", "none");

    // legand
    const legand = chart.append('g');
    const legandWidth = W * 0.25;
    const legandHeight = H * 0.17;

    legand.append("rect")
        .attr("x", X + W * 0.02)
        .attr("y", Y)
        .attr("width", legandWidth)
        .attr("height", legandHeight)
        .attr("fill", "white")
        .style("stroke-width", "1px")
        .style("stroke", "#00000060");

    legand.append("circle")
        .attr("cx", X + W * 0.04)
        .attr("cy", Y + H * 0.03)
        .attr("r", 3)
        .style("fill", colorMapper["ptsNorm"]);
    legand.append("text")
        .attr("x", X + W * 0.06)
        .attr("y", Y + H * 0.04)
        .attr("font-size", "15px")
        .text("pts")
        .style("color", "black");
    legand.append("circle")
        .attr("cx", X + W * 0.14)
        .attr("cy", Y + H * 0.03)
        .attr("r", 3)
        .style("fill", colorMapper["rebNorm"]);
    legand.append("text")
        .attr("x", X + W * 0.16)
        .attr("y", Y + H * 0.04)
        .attr("font-size", "15px")
        .text("reb")
        .style("color", "black");
    legand.append("circle")
        .attr("cx", X + W * 0.04)
        .attr("cy", Y + H * 0.12)
        .attr("r", 3)
        .style("fill", colorMapper["astNorm"]);
    legand.append("text")
        .attr("x", X + W * 0.06)
        .attr("y", Y + H * 0.13)
        .attr("font-size", "15px")
        .text("ast")
        .style("color", "black");
    legand.append("text")
        .attr("x", X + W * 0.14)
        .attr("y", Y + H * 0.13)
        .text("r")
        .style("color", "black");
    legand.append("text")
        .attr("x", X + W * 0.16)
        .attr("y", Y + H * 0.13)
        .attr("font-size", "15px")
        .text("avg")
        .style("color", "black");

    // brush
    const brush = d3.brush().extent([[X, Y], [X + W, Y + H]])
        .on("brush", () => {
            const extent = d3.event.selection;
            onClickPlayerDataGraph = false;
            playerData = [];

            for (const key of Object.keys(locMap)) {
                locMap[key].selectTot = 0;
            }

            data.forEach(
                (d) => {
                    const x = xTicks(d.umapX) + X;
                    const y = yTicks(d.umapY) + Y;
                    if(x >= extent[0][0] && x <= extent[1][0] && y >= extent[0][1] && y <= extent[1][1]) {
                        playerData.push(d);
                        locMap[d.team_abbreviation].selectTot += 1;
                    }

                }
            )
        })
        .on("end", () => {
            onClickPlayerDataGraph = false;
            redrawBarChartHandler();
            redrawAmericaDot();
        });


    playerGraph.on('click', () => {
        onClickPlayerDataGraph = true;
        redrawBarChartHandler();
        redrawAmericaDot();
    }).call(brush);

    // dots
    dots = playerGraph.append('g')
        .selectAll("dot")
        .data(data)
        .enter()
        .append("circle")
        .attr("cx", d => xTicks(d.umapX) + X)
        .attr("cy", d => yTicks(d.umapY) + Y)
        .attr("r", d => sizeTicks(d[getMax(d)]))
        .style("fill", d => colorMapper[getMax(d)]).call(tips)
        .on('mouseover', tips.show).on('mouseout', tips.hide);


    return data;

}

function redrawBarChart(data, term, W, H, X, Y) {

    const barChart = barCharts.append("g");

    const xTicks = d3.scaleLinear().domain(d3.extent(origPlayerData, d => d[term])).range([0, W]);


    const histogram = d3.histogram()
        .value(d => d)
        .domain(xTicks.domain())
        .thresholds(xTicks.ticks(70));

    const binsNew = onClickPlayerDataGraph ? histogram(origPlayerData.map(d => d[term])) : histogram(data);
    const binsOrig = histogram(origPlayerData.map(d => d[term]));


    const yTicks = d3.scaleLinear().domain(d3.extent(binsOrig, d => d.length)).range([H, 0]);

    // barChart
    barChartsMapper[term].data(binsNew).transition(1000).attr("transform", d => `translate(${xTicks(d.x0) + X}, ${yTicks(d.length) + Y})`).attr("height", d => H - yTicks(d.length))

}


function redrawBarChartHandler() {
    redrawBarChart(playerData.map(d => d["pts"]), "pts",WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20, MARGIN.TOP + HEIGHT / 2 + 10);
    redrawBarChart(playerData.map(d => d["reb"]), "reb", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20 + WIDTH / 3, MARGIN.TOP + HEIGHT / 2 + 10);
    redrawBarChart(playerData.map(d => d["ast"]), "ast", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20 + WIDTH / 3 * 2, MARGIN.TOP + HEIGHT / 2 + 10);
    redrawBarChart(playerData.map(d => d["age"]), "age", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20, MARGIN.TOP + HEIGHT / 1.3 + 10);
    redrawBarChart(playerData.map(d => d["gp"]), "gp", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20 + WIDTH / 3, MARGIN.TOP + HEIGHT / 1.3 + 10);
    redrawBarChart(playerData.map(d => d["draft_number"]), "draft_number", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20 + WIDTH / 3 * 2, MARGIN.TOP + HEIGHT / 1.3 + 10);
}

function getPeriodPoints() {
    // Initialize
    for (const key of Object.keys(locMap)) {
        locMap[key].selectTot = 0;
    }

    playerData = [];

    // getAllValidData
    origPlayerData.forEach(
        (d) => {
            function checkCondition() {
                for (const [term, periods] of Object.entries(currentBars)) {
                    var inPeriod = periods.length === 0;
                    for(const period of periods) {
                        if(d[term] >= period[0] && d[term] <= period[1]) {
                            inPeriod = true;
                        }
                    }
                    if(!inPeriod)
                        return false;

                }
                return true;
            }

            if(checkCondition()) {
                d.show = true;
                playerData.push(d);
                locMap[d.team_abbreviation].selectTot += 1;
            } else {
                d.show = false;
            }

        }
    )

}

function redrawScatterPlotHandler() {
    dots.attr('opacity', d => d.show ? 1.0 : 0.5);
}


function drawBarChart(data, term, W, H, X, Y) {

    const barChartFront = barCharts.append("g");
    const barChartBack = barCharts.append("g");

    const xTicks = d3.scaleLinear().domain(d3.extent(data, d => d)).range([0, W]);

    const histogram = d3.histogram()
        .value(d => d)
        .domain(xTicks.domain())
        .thresholds(xTicks.ticks(70));

    const bins = histogram(data);

    const yTicks = d3.scaleLinear().domain(d3.extent(bins, d => d.length)).range([H, 0]);


    // x label
    barChartBack.append("g")
        .attr("transform", `translate(${X},  ${Y + H})`)
        .call(d3.axisBottom(xTicks));

    // y label
    barChartBack.append("g")
        .attr("transform", `translate(${X},  ${Y})`)
        .call(d3.axisLeft(yTicks));

    // barChart
    barChartBack.selectAll("rect")
        .data(bins)
        .enter()
        .append("rect")
        .attr("x", 1)
        .attr("transform", d => `translate(${xTicks(d.x0) + X}, ${yTicks(d.length) + Y})`)
        .attr("width", d => xTicks(d.x1) - xTicks(d.x0))
        .attr("height", d => H - yTicks(d.length))
        .attr("stroke", "black")
        .style("fill", "#FFFFFF").style("fill-opacity", 0);

    barChartsMapper[term] = barChartFront.selectAll("rect")
        .data(bins)
        .enter()
        .append("rect")
        .attr("x", 1)
        .attr("transform", d => `translate(${xTicks(d.x0) + X}, ${yTicks(d.length) + Y})`)
        .attr("width", d => xTicks(d.x1) - xTicks(d.x0))
        .attr("height", d => H - yTicks(d.length))
        .style("fill", "#69b3a2");


    // callback
    const brush = d3.brushX().extent([[X, Y], [X + W, Y + H]])
        .on("brush", () => {
            const extent = d3.event.selection;
            currentBars[term] = [];

            bins.forEach((d) => {
                const x0 = xTicks(d.x0) + X;

                if(x0 >= extent[0] && x0 <= extent[1]) {
                    currentBars[term].push([d.x0, d.x1]);
                }
            });
        })
        .on("end", () => {
            if(!showMap[term])
                ++showCnt;
            showMap[term] = true;
            onClickPlayerDataGraph = false;
            getPeriodPoints();
            redrawScatterPlotHandler();
            redrawAmericaDot();
            redrawBarChartHandler();
        });

    barChartBack.on('click', () => {
        if(showMap[term])
            --showCnt;
        showMap[term] = false;
        onClickPlayerDataGraph = showCnt === 0;
        currentBars[term] = [];
        getPeriodPoints();
        redrawScatterPlotHandler();
        redrawAmericaDot();
        redrawBarChartHandler();
    }).call(brush);


    // legand
    chart.append("text")
        .attr("x", X + W * 0.76)
        .attr("y", Y + H * 0.14)
        .attr("font-size", "15px")
        .text(term);
}

async function barChartHandler(data) {
    drawBarChart(data.map(d => d["pts"]), "pts",WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20, MARGIN.TOP + HEIGHT / 2 + 10);
    drawBarChart(data.map(d => d["reb"]), "reb", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20 + WIDTH / 3, MARGIN.TOP + HEIGHT / 2 + 10);
    drawBarChart(data.map(d => d["ast"]), "ast", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20 + WIDTH / 3 * 2, MARGIN.TOP + HEIGHT / 2 + 10);
    drawBarChart(data.map(d => d["age"]), "age", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20, MARGIN.TOP + HEIGHT / 1.3 + 10);
    drawBarChart(data.map(d => d["gp"]), "gp", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20 + WIDTH / 3, MARGIN.TOP + HEIGHT / 1.3 + 10);
    drawBarChart(data.map(d => d["draft_number"]), "draft_number", WIDTH / 3.2, HEIGHT / 5, MARGIN.LEFT + 20 + WIDTH / 3 * 2, MARGIN.TOP + HEIGHT / 1.3 + 10);
    return data;
}

async function playerNumHandler(data) {
    return data;
}


async function teamLocDataExtractor(csvData) {
    csvData.forEach( data => {
        data.lon = Number(data.lon);
        data.lat = Number(data.lat);
        locMap[data.team_abbreviation] = {lon: data.lon, lat: data.lat, tot: 0, selectTot: 0};
    });
    return csvData;
}

function redrawAmericaDot() {
    const sizeTicks = d3.scaleLinear().domain([0, 10, 20]).range([0, 15, 17]);

    if(!onClickPlayerDataGraph)
        dotsInAmerica.transition(1000).attr("r", d => sizeTicks(d[1].selectTot));
    else
        dotsInAmerica.transition(1000).attr("r", d => sizeTicks(d[1].tot));
}


async function drawAmerica(data) {
    const W = 0.6 * WIDTH;
    const H = 0.5 * HEIGHT;

    const L = MARGIN.LEFT + 0.42 * WIDTH;
    const U = MARGIN.TOP;

    const projection = d3.geoEquirectangular()
        .fitExtent([[L + 0, U + 0], [L + W, U + H]], data);

    const geoGenerator = d3.geoPath().projection(projection);

    const map = chart.append('g');
    const paths = map.selectAll('path')
        .data(data.features)
        .enter()
        .append('path')
        .attr('stroke', 'black')
        .attr('fill', 'white')
        .attr('d', geoGenerator)

    const texts = map.selectAll('text')
        .data(Object.entries(locMap))
        .enter()
        .append('text')
        .attr('text-anchor', 'middle')
        .attr('alignment-baseline', 'middle')
        .attr('font-size', '15px')
        .attr("x", d => projection([d[1].lon, d[1].lat])[0])
        .attr("y", d => projection([d[1].lon, d[1].lat])[1])
        .text(d => d[0])


    const sizeTicks = d3.scaleLinear().domain([10, 20]).range([15, 17]);

    dotsInAmerica = map.append('g')
        .selectAll('circle')
        .data(Object.entries(locMap))
        .enter()
        .append('circle')
        .attr('opacity', 0.5)
        .attr("cx", d => projection([d[1].lon, d[1].lat])[0])
        .attr("cy", d => projection([d[1].lon, d[1].lat])[1])
        .attr("r", d => sizeTicks(d[1].tot))
        .style("fill", "red");

}

async function teamLocDataHandler(data) {

}

d3.csv("TeamLoc.csv").then( teamLocDataExtractor ).then( teamLocDataHandler ).catch(err => console.log(err));
d3.csv("NBA1516.csv").then( playerDataExtractor ).then( scatterPlotHandler ).then( barChartHandler ).then( playerNumHandler ).catch(err => console.log(err));
d3.json("us-states.json").then(drawAmerica).catch(err => console.log(err));