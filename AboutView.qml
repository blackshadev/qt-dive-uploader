import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

GridLayout {
    Layout.fillWidth: true
    columns: 2


    Text {
        Layout.fillWidth: true
        wrapMode: Text.Wrap
        Layout.bottomMargin: 44
        Layout.columnSpan: 2
        text: "This tool is an opensource tool for extracting and uploading uploading dives to an homebrew open-source online divelog."
    }

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
