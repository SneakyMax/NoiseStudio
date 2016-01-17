import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "qrc:/scripts/graph.js" as Graph
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

    Item {
        id: graphArea
        anchors.fill: parent

        Rectangle {
            id: graphGridBackground
            color: "#3b3b3b"
            anchors.fill: parent
        }

        Canvas {
            id: graphGrid
            anchors.fill: parent
            Component.onCompleted: GraphGrid.initGrid(Graph, graphGrid)
            onPaint: GraphGrid.drawGrid()

            renderTarget: Canvas.FramebufferObject
            renderStrategy: Canvas.Cooperative
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.MiddleButton | Qt.LeftButton | Qt.RightButton
            onWheel: Graph.handleMouseWheel(wheel)
            onPressed: Graph.handleMousePress(mouse)
            onReleased: Graph.handleMouseRelease(mouse)
            onPositionChanged: Graph.handleMouseMove(mouse)
        }

        Menu {
            id: graphContextMenu

            title: "Title"

            style: MenuStyle {
                frame: Rectangle {
                    color: "#3f3f3f"
                    border.width: 1
                    border.color: "#141414"
                }

                itemDelegate.background: Rectangle {
                    color: "#585858"
                    visible: styleData.selected
                }

                itemDelegate.label: Text {
                    color: "#c8c8c8"
                    text: styleData.text
                    font.pointSize: 10
                }
            }

            Menu {
                title: "Add"

                MenuItem {
                    text: "Perlin Noise"
                }

                MenuItem {
                    text: "Math"
                }
            }

            MenuItem {
                text: "Remove"
            }
        }
    }
}
