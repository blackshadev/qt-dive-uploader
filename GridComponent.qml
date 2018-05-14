import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3


Item {
    property int iNumcolums: 3
    property int iMarginGrid: 2

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
    }

    ListView {
        clip: false
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        anchors.fill: parent
        ScrollBar.vertical: ScrollBar {}

        model: appModel


        delegate: Item {
            width: parent.width
            height: 25

            RowLayout {
                Text {
                    text: colorR
                }
            }

        }

    }

}
