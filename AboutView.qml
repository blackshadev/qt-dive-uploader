import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.12
import FontAwesome 1.0

ColumnLayout {
    anchors.fill: parent

    GridLayout {
        columns: 2
        Layout.alignment: Qt.AlignCenter
        Layout.topMargin: 35

        Label {
            text: "Author"
        }

        Label {
            text: "Vincent Hagen"
        }

        Label {
            text: "Build date"
        }

        Label {
            text: "2018-06-13"
        }

        Label {
            text: "Libdivecomputer version"
        }

        Label {
            text: libdivecomputer.version
        }
    }

    Text {
        Layout.fillWidth: true
        wrapMode: Text.Wrap
        Layout.topMargin: 22
        text: "This tool is an opensource tool for extracting, downloading and uploading dives to an homebrew open-source online divelog. You can view the sources and contribute on github"
    }

    RowLayout {
        Layout.fillWidth: true

        RoundButton {
            Material.elevation: 0

            text: FontAwesome.github
            font.family: FontAwesome.fontFamily
            font.pixelSize: 22
            onClicked: {
                Qt.openUrlExternally("https://github.com/blackshadev/qt-dive-uploader");
            }

            hoverEnabled: true
            ToolTip.timeout: 5000
            ToolTip.text: "Open github project page"
            ToolTip.visible: hovered

            MouseArea {
                anchors.fill: parent
                cursorShape: "PointingHandCursor"
            }
        }

        RoundButton {
            Material.elevation: 0

            text: FontAwesome.globe
            font.family: FontAwesome.fontFamily
            font.pixelSize: 22
            onClicked: {
                Qt.openUrlExternally("https://dive.littledev.nl/");
            }


            hoverEnabled: true
            ToolTip.timeout: 5000
            ToolTip.text: "Open divelog website"
            ToolTip.visible: hovered


            MouseArea {
                anchors.fill: parent
                cursorShape: "PointingHandCursor"
            }
        }
    }



}
