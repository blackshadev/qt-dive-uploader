import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0
import SortFilterProxyModel 0.1
import DCComputer 0.1
import Libdivecomputer 0.1


GridLayout {
    Layout.fillWidth: true
    columns: 2

    FileDialog {
        visible: false
        id: fileDialog
        selectExisting: false
        folder: shortcuts.home
        title: "Please choose a file to save"
        nameFilters: [ "JSON files (*.json)", "All files (*)" ]
        onAccepted: {
            var text = /^file:\/\/(.*)$/.exec(fileDialog.fileUrl)[1];
            if(!/\.json$/.test(text)) {
                text += ".json";
            }

            filePath.text = text;
            session.path = text;
            libdivecomputer.path = text;

            fileDialog.close();
        }
        onRejected: {
            console.log("Canceled")
            fileDialog.close();

        }
    }


    Label {
        text: "Port"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    ComboBox {
        Layout.fillWidth: true

        property bool loaded: false
        id: portSelection
        editable: true
        model: libdivecomputer.ports
        Component.onCompleted: {
            var idx = portSelection.find(session.portname);
            if(idx > -1) {
                portSelection.currentIndex = idx;
            }
            loaded = true;
        }
        onCurrentIndexChanged: {
            if(loaded) {
                session.portname = portSelection.textAt(portSelection.currentIndex);
            }
        }

    }

    Label {
        text: "Computer"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    ComboBox {
        Layout.fillWidth: true

        property bool loaded: false
        id: computerSelection
        editable: true
        model: SortFilterProxyModel {
            sourceModel: libdivecomputer.devices
            sortRoleName: "description"
            dynamicSortFilter: true
            sortOrder: "AscendingOrder"
        }
        textRole: "description"
        Component.onCompleted: {
            var idx = computerSelection.find(session.computer);

            if(idx > -1) {
                computerSelection.currentIndex = idx;
            }
            loaded = true;
        }
        onCurrentIndexChanged: {
            if(loaded) {
                session.computer = computerSelection.textAt(computerSelection.currentIndex);
            }
        }

    }

    Label {
        text: "Output Type"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    RowLayout {
        id: outputType
        Layout.fillWidth: true

        RadioButton {
            text: "File"
            id: fileRadio
            checked: libdivecomputer.writeType === WriteTypes.File
            onCheckedChanged: {
                if(fileRadio.checked) {
                    libdivecomputer.writeType = WriteTypes.File;
                }
            }
        }

        RadioButton {
            text: "LittleLog"
            id: llRadio
            checked: libdivecomputer.writeType === WriteTypes.LittleLog
            enabled: littledivelog.userInfo !== null
            onCheckedChanged: {
                if(llRadio.checked) {
                    libdivecomputer.writeType = WriteTypes.LittleLog;
                }
            }
        }
    }

    Label {
        text: "Output file"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    RowLayout {
        Layout.fillWidth: true

        TextField {
            Layout.fillWidth: true

            id: filePath
            text: session.path
            readOnly: true
            enabled: fileRadio.checked
        }

        Button {
            text: "Browse..."
            onClicked: {
                fileDialog.open();
            }

            enabled: fileRadio.checked
        }
    }


    ProgressBar {
        id: readProgress
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }

    ProgressBar {
        id: writeProgress
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }

    Button {
        id: startButton
        Layout.columnSpan: 2
        Layout.alignment: Qt.AlignRight
        text: "Start"
        onClicked: {

            writeProgress.value = 0;
            readProgress.value = 0;

            var idx = computerSelection.model.index(computerSelection.currentIndex, 0);
            libdivecomputer.start_download(
                portSelection.currentText,
                computerSelection.model.data(idx, ComputerRoles.IndexRole)
            );

        }
    }


    Label {
        id: errorLabel
        Layout.columnSpan: 2
        Layout.fillWidth: true
        color: "red"
        text: ""
    }

    Label {
        text: "DC Version"
    }

    Label {
        Layout.fillWidth: true
        text: libdivecomputer.version
    }

    Connections {
        target: libdivecomputer
        onReadProgress: {
            readProgress.value = current / total;
        }

        onWriteProgress: {
            writeProgress.value = current / total;
        }

        onStart: {
            startButton.enabled = false;
        }
        onDone: {
            startButton.enabled = true;
        }
        onError: {
            console.log(msg);
            errorLabel.text = msg;
        }
    }
}
