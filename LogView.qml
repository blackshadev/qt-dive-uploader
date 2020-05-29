import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
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
        model: libdivecomputer.LogLevels
        Component.onCompleted: {
            loaded = true;
            var idx = loglevelSelector.find(libdivecomputer.logLevel);
            if(idx > -1) {
                loglevelSelector.currentIndex = idx;
            }
        }

        onCurrentTextChanged: {
            if(loaded) {
                libdivecomputer.logLevel = loglevelSelector.currentText;
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
        function onLog(lvl, msg) { log(lvl, msg); }
    }

}
