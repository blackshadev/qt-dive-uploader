import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

ColumnLayout {

    GridLayout {
        Layout.fillWidth: true
        columns: 2

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
        Layout.topMargin: 44
        text: "This tool is an opensource tool for extracting and uploading uploading dives to an homebrew open-source online divelog."
    }

}
