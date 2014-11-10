var start = -10000,
    end = 10000;

var currentGraph = {};

function initGrid() {
    currentGraph.context = graphGrid.getContext("2d");
    currentGraph.context.lineWidth = 1;

    currentGraph.scale = 70; //70px for 1 "unit" default
    currentGraph.translation = { left: 0, top: 0 };

    drawGrid();
}

function drawGrid() {
    drawMinorGridlines();
    drawMajorGridlines();
}

function drawMinorGridlines() {
    if(scale < 35)
        return;

    var context = currentGraph.context;

    context.beginPath();
    context.strokeStyle = "rgb(49, 49, 49)";
    for(var i = start; i < end; i += (scale / 4)) {
        var aligned = Math.floor(i) + 0.5;
        context.moveTo(start, aligned);
        context.lineTo(end, aligned);
        context.moveTo(aligned, start);
        context.lineTo(aligned, end);
    }
    context.stroke();
    context.closePath();
}

function drawMajorGridlines() {
    if(scale < 3)
        return;

    var context = currentGraph.context;

    context.beginPath();
    context.strokeStyle = "rgb(43, 43, 43)";
    for(var i = start; i < end; i += scale) {
        var aligned = Math.floor(i) + 0.5;
        context.moveTo(start, aligned);
        context.lineTo(end, aligned);
        context.moveTo(aligned, start);
        context.lineTo(aligned, end);
    }
    context.stroke();
    context.closePath();
}
