import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import SortFilterProxyModel 0.1
import DCComputer 0.1
import Libdivecomputer 0.1
import QtQuick.Controls.Material 2.2
import FontAwesome 1.0

GridLayout {
    function ensureJSON(filepath) {
        if(!/\.json$/.test(filepath)) {
            filepath = filepath.replace(/\..*$/, "");
            filepath += ".json";
        }
        return filepath;
    }

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
            var text = fileDialog.fileUrl.toString().replace(/^(file:\/{3})/, "");
            text = decodeURIComponent(text);
            text = ensureJSON(text);

            filePath.text = text;
            session.path = text;
            libdivecomputer.path = text;

            fileDialog.close();
        }
        onRejected: {
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
            sortOrder: "AscendingOrder"
        }
        textRole: "description"
        valueRole: ComputerRoles.IndexRole
        Component.onCompleted: {
            var idx = computerSelection.find(session.computer);

            if(idx > -1) {
                computerSelection.currentIndex = idx;
            }
            loaded = true;
        }
        onCurrentIndexChanged: {

            var idx = computerSelection.model.index(computerSelection.currentIndex, 0);
            libdivecomputer.set_available_transports(computerSelection.model.data(idx, ComputerRoles.TransportsRole ));

            if(loaded) {
                session.computer = computerSelection.currentText;
            }
        }

    }

    Label {
        text: "Protocol"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    ComboBox {
        Layout.fillWidth: true
        model: libdivecomputer.transports
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
        text: "Select Dives"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    CheckBox {
        id: selectDives
        Component.onCompleted: {
            checked = session.selectDives;
        }
        onCheckStateChanged: {
            session.selectDives = checked;
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
            enabled: fileRadio.checked
            onTextChanged: {
                var fixedText = ensureJSON(filePath.text);
                if(filePath.text !== fixedText) {
                    filePath.text = fixedText;
                }

                session.path = fixedText;
                libdivecomputer.path = fixedText;
            }
        }

        Button {
            Material.elevation: 0
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


    Label {
        Layout.fillWidth: true
        id: errorLabel
        color: "red"
        text: ""
    }

    RoundButton {
        id: startButton
        Layout.alignment: Qt.AlignRight
        text: FontAwesome.download

        font.family: FontAwesome.fontFamily
        font.pointSize: 25
        padding: 20
        enabled: libdivecomputer.isReady

        Component.onCompleted: {
            startButton.background.color = Material.color(Material.Blue)
            startButton.contentItem.color = Material.color(Material.Grey, Material.Shade200)
        }

        onEnabledChanged: {
            startButton.background.color = enabled ? Material.color(Material.Blue) : Material.color(Material.Blue, Material.Shade300);
            startButton.contentItem.color = enabled ?   Material.color(Material.Grey, Material.Shade200) :  Material.color(Material.Grey, Material.Shade300);
        }

        onClicked: {

            errorLabel.text = "";
            writeProgress.value = 0;
            readProgress.value = 0;

            var idx = computerSelection.model.index(computerSelection.currentIndex, 0);
            libdivecomputer.start_download(
                portSelection.currentText,
                computerSelection.model.data(idx, ComputerRoles.IndexRole),
                selectDives.checked
            );

        }
    }



    Connections {
        target: libdivecomputer
        function onReadProgress(current, total) {
            readProgress.value = current / total;
        }

        function onWriteProgress(current, total) {
            writeProgress.value = current / total;
        }

        function onStart() {
            startButton.enabled = false;
        }
        function onDone() {
            startButton.enabled = true;
        }

        function onError(msg) {
            errorLabel.text = msg;
        }
    }
}
