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
        anchors.top: loglevelSelector.bottom;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        font.pointSize: 10

        readOnly: true
        text: logText
    }

}
