import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

ColumnLayout {
    property string logText: ""
    function log(lvl, msg) {
        logText += msg + "\n";
    }

    Layout.fillWidth: true
    Layout.fillHeight: true

    ComboBox {
        Layout.fillWidth: true
        id: loglevelSelector
        model: ListModel {
            id: logLevelModel
            ListElement { text: "All" }
            ListElement { text: "Debug" }
            ListElement { text: "Info" }
            ListElement { text: "Warning" }
            ListElement { text: "Error" }
            ListElement { text: "None" }
        }
    }


    TextArea {
        anchors.fill: parent
        font.pointSize: 10

        readOnly: true
        text: logText
    }

}
