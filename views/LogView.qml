import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import Libdivecomputer 0.2

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
        model: QDCLogLevelListModel {}
        Component.onCompleted: {
            loaded = true;
//            var idx = loglevelSelector.find(libdivecomputer.logLevel);
//            if(idx > -1) {
//                loglevelSelector.currentIndex = idx;
//            }
        }

        onCurrentValueChanged: {
            if(loaded) {
                dccontext.loglevel = loglevelSelector.currentValue;
            }
        }
        valueRole: "value"
        textRole: "description"
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
        target: dccontext
        function onLog(lvl, msg) { log(lvl, msg); }
    }

}
