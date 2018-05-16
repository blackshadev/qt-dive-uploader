import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

Window {
    id: selectionWindow
    width: 640
    height: 480

    ColumnLayout {
        anchors.fill: parent

        ListModel {
            id: appModel

            ListElement { colorR: "red"}
            ListElement { colorR: "green" }
            ListElement { colorR: "blue" }
            ListElement { colorR: "cyan"}
            ListElement { colorR: "yellow"}
            ListElement { colorR: "blue" }
            ListElement { colorR: "lightgray" }
            ListElement { colorR: "red" }
            ListElement { colorR: "green" }
            ListElement { colorR: "blue" }
            ListElement { colorR: "cyan" }
            ListElement { colorR: "yellow" }
            ListElement { colorR: "lightgray" }
            ListElement { colorR: "blue" }

            ListElement { colorR: "red"; selected: true }
            ListElement { colorR: "green"; selected: true }
            ListElement { colorR: "blue"; selected: true }
            ListElement { colorR: "cyan"; selected: true }
            ListElement { colorR: "yellow"; selected: true }
            ListElement { colorR: "blue"; selected: true }
            ListElement { colorR: "lightgray"; selected: true }
            ListElement { colorR: "red"; selected: true }
            ListElement { colorR: "green"; selected: true }
            ListElement { colorR: "blue"; selected: true }
            ListElement { colorR: "cyan"; selected: true }
            ListElement { colorR: "yellow"; selected: true }
            ListElement { colorR: "lightgray"; selected: true }
            ListElement { colorR: "blue"; selected: true }
        }

        ListView {

            id: listView
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            anchors.fill: parent
            ScrollBar.vertical: ScrollBar {}

            model: appModel

            headerPositioning: ListView.OverlayHeader
            header: Rectangle {

                width: parent.width
                height: 32

                z: 2
                color: "#cccccc"
                RowLayout {
                    anchors.fill: parent
                    spacing: 10

                    Text {
                        text: ""
                    }

                    Text {
                        text: "Date"
                    }

                    Text {
                        text: "DiveTime"
                    }

                    Text {
                        text: "Depth"
                    }
                }
            }

            delegate: ItemDelegate {
                width: parent.width
                height: 32

                checkable: true
                highlighted: selected
                onClicked: selected = !selected


                contentItem: RowLayout {
                    spacing: 10

                    Text {
                        text: colorR
                    }

                    Text {
                        text: colorR
                    }

                    Text {
                        text: colorR
                    }
                }

            }

        }
    }

}
