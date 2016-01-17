var graph = {
    scale: 70,
    scaleFactor: 70, //70px for 1 "unit"
    translation: { left: 0, top: 0 },
    distance: 1
};

function getModel() {
    return graph;
}

/** Public Api **/

function handleMouseWheel(wheelEvent) {
    var units = wheelEvent.angleDelta.y,
        modifier = 1 / 2500;

    zoom(-1 * units * modifier);
}

function handleMousePress(mouse) {
    if(mouse.button === Qt.MiddleButton) {
        setDragAnchor(mouse);
    }
}

function handleMouseRelease(mouse) {
    if(mouse.button === Qt.RightButton) {
        openContextMenu(mouse);
    }
}

function handleMouseMove(mouse) {
    if(mouse.buttons & Qt.MiddleButton) {
        translate({ left: mouse.x - graph.startDragPosition.x, top: mouse.y - graph.startDragPosition.y });
    }
}

function onScaleChanged(handler) {
    scaleChangedHandlers.push(handler);
}

function onTranslationChanged(handler) {
    translationChangedHandlers.push(handler);
}

/** Private Api **/

var openContextMenu = function(mouse) {
    graphContextMenu.popup();
};

var setDragAnchor = function(mouse) {
    graph.startTranslation = { left: graph.translation.left, top: graph.translation.top };
    graph.startDragPosition = { x: mouse.x, y: mouse.y };
};

var zoom = function(change) {
    graph.distance += change;
    graph.distance = Math.max(0.00001, graph.distance);

    graph.scale = (1 / graph.distance) * graph.scaleFactor;

    scaleChanged();
};

var translate = function(delta) {
    var deltaLocal = { left: delta.left / graph.scale, top: delta.top / graph.scale };

    graph.translation = { left: graph.startTranslation.left + deltaLocal.left, top: graph.startTranslation.top + deltaLocal.top };
    translationChanged();
};

var scaleChangedHandlers = [];
var scaleChanged = function() {
    for(var i = 0; i < scaleChangedHandlers.length; i++)
        scaleChangedHandlers[i](graph.scale);
};

var translationChangedHandlers = [];
var translationChanged = function() {
    for(var i = 0; i < translationChangedHandlers.length; i++)
        translationChangedHandlers[i](graph.translation);
};
