import QtQuick 2.3
import QtQuick.Controls 1.2
import "qrc:/scripts/graphGrid.js" as GraphGrid

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Rectangle {
        id: graphGridBackground
        color: "#3b3b3b"
        anchors.fill: parent
    }

    Canvas {
        id: graphGrid
        anchors.fill: parent
        Component.onCompleted: GraphGrid.initGrid(graphGrid)
        onPaint: GraphGrid.drawGrid()

        renderTarget: Canvas.FramebufferObject
        renderStrategy: Canvas.Cooperative

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.MiddleButton
            onWheel: GraphGrid.zoom(wheel)
            onPressed: GraphGrid.handleMousePress(mouse)
            onPositionChanged: GraphGrid.handleMouseMove(mouse)
        }
    }
}
