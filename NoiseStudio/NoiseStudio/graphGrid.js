var graph = {
    scale: 70,
    scaleFactor: 70, //70px for 1 "unit"
    translation: { left: 0, top: 0 },
    distance: 1
};

/** Public **/

function initGrid(canvas) {
    graph.canvas = canvas;
    graph.context = canvas.getContext("2d");
    graph.context.lineWidth = 1;

    drawGrid();
}

function drawGrid() {
    graph.width = graph.canvas.canvasSize.width;
    graph.height = graph.canvas.canvasSize.height;

    graph.context.clearRect(0, 0, graph.width, graph.height);

    graph.localWidth = graph.width / graph.scale;
    graph.localHeight = graph.height / graph.scale;

    graph.effectiveTranslation = {
        left: graph.translation.left + (graph.localWidth / 2),
        top: graph.translation.top + (graph.localHeight / 2)
    };

    drawMinorGridlines();
    drawMajorGridlines();
    drawAxes();
}

function zoom(wheelEvent) {
    var units = wheelEvent.angleDelta.y,
        modifier = 1 / 2500;

    graph.distance += -1 * units * modifier;
    graph.distance = Math.max(0.00001, graph.distance);

    graph.scale = (1 / graph.distance) * graph.scaleFactor;

    redraw();
}

function handleMousePress(mouse) {
    if(mouse.button !== Qt.MiddleButton)
        return;

    graph.startTranslation = { left: graph.translation.left, top: graph.translation.top };
    graph.startDragPosition = { x: mouse.x, y: mouse.y };
}

function handleMouseMove(mouse) {
    if(!(mouse.buttons & Qt.MiddleButton))
        return;

    var delta = { left: mouse.x - graph.startDragPosition.x, top: mouse.y - graph.startDragPosition.y };
    var deltaLocal = { left: delta.left / graph.scale, top: delta.top / graph.scale };

    graph.translation = { left: graph.startTranslation.left + deltaLocal.left, top: graph.startTranslation.top + deltaLocal.top };

    redraw();
}

/** Private **/

function redraw() {
    graph.canvas.requestPaint();
}

function drawMinorGridlines() {
    if(graph.scale < 35)
        return;

    drawGridlines(4, "rgb(49, 49, 49)");
}

function drawMajorGridlines() {
    if(graph.scale < 3)
        return;

    drawGridlines(1, "rgb(43, 43, 43)");
}

function drawAxes() {
    drawGridlines(0.0000001, "rgb(35, 35, 35)");
}

function drawGridlines(frequency, color) {
    var context = graph.context,
        i, aligned, screenSpace;

    context.beginPath();
    context.strokeStyle = color;

    //columns
    var period = 1 / frequency,
        start = -graph.translation.left - (graph.localWidth / 2),
        first = Math.ceil(start * frequency) / frequency,
        end = start + graph.localWidth,
        translation = graph.effectiveTranslation.left;

    for(i = first; i < end; i += period) {
        screenSpace = (i + translation) * graph.scale;
        aligned = Math.round(screenSpace) + 0.5;
        context.moveTo(aligned, 0);
        context.lineTo(aligned, graph.height);
    }

    //rows
    start = -graph.translation.top - (graph.localHeight / 2);
    first = Math.ceil(start * frequency) / frequency;
    end = start + graph.localHeight;
    translation = graph.effectiveTranslation.top;

    for(i = first; i < end; i+= period) {
        screenSpace = (i + translation) * graph.scale;
        aligned = Math.round(screenSpace) + 0.5;
        context.moveTo(0, aligned);
        context.lineTo(graph.width, aligned);
    }

    context.stroke();
    context.closePath();
}
