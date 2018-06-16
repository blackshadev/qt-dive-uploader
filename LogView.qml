import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

ColumnLayout {

    property string logText: ""
    function log(lvl, msg) {
        logText += msg + "\n";
    }

    Layout.fillWidth: true
    Layout.fillHeight: true

    ComboBox {
        property bool loaded: false
        Layout.fillWidth: true
        id: loglevelSelector
        model: libdivecomputer.loglevels
        Component.onCompleted: {
            loaded = true;
            var idx = loglevelSelector.find(libdivecomputer.loglevel);
            if(idx > -1) {
                loglevelSelector.currentIndex = idx;
            }
        }

        onCurrentTextChanged: {
            if(loaded) {
                libdivecomputer.loglevel = loglevelSelector.currentText;
            }
        }
    }

    TextArea {
        Layout.fillHeight: true
        Layout.fillWidth: true

        font.pointSize: 10
        padding: 7

        readOnly: true
        text: logText
        background: Rectangle {
            color: "#fefefe"
            border.color: Material.color(Material.Grey)
        }

    }

    Connections {
        target: libdivecomputer
        onLog: log(lvl, msg)
    }

}
