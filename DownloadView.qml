import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import SortFilterProxyModel 0.1
import DCComputer 0.1
import DCTransport 0.1
import Libdivecomputer 0.1
import QtQuick.Controls.Material 2.12
import FontAwesome 1.0

GridLayout {

    enum Stages {
        None = 0,
        ComputerSelection = 1,
        TransportSelection = 2,
        SourceSelection = 3,
        OutputSelection = 4
    }

    function isValid(stage) {
        var validStages = 1 << DownloadView.Stages.None;

        if(computerSelection.currentIndex > -1) {
            validStages |= 1 << DownloadView.Stages.ComputerSelection;
        }
        if(transportSelection.currentIndex > -1) {
            validStages |= 1 << DownloadView.Stages.TransportSelection;
        }
        if(sourceSelection.currentIndex > -1) {
            validStages |= 1 << DownloadView.Stages.SourceSelection;
        }

        if(fileRadio.checked && filePath.text !== "") {
            validStages |= 1 << DownloadView.Stages.OutputSelection;
        } else if(llRadio.checked && littledivelog.isLoggedIn) {
            validStages |= 1 << DownloadView.Stages.OutputSelection;
        }

        var mask  = (1 << (stage + 1)) -1;

        return (validStages & mask) === mask;
    }

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
        valueRole: "index"
        Component.onCompleted: {
            var idx = computerSelection.find(session.computer);

            if(idx > -1) {
                computerSelection.currentIndex = idx;
            }
            loaded = true;
        }
        onCurrentIndexChanged: {

            var idx = computerSelection.model.index(computerSelection.currentIndex, 0);

            var comp = "";
            var trans = 0;

            if(idx.valid) {
                comp = computerSelection.model.data(idx, ComputerRoles.DescriptionRole);
                trans = computerSelection.model.data(idx, ComputerRoles.TransportsRole);
            }

            libdivecomputer.set_available_transports(trans);

            if(loaded) {
                session.computer = comp;
            }

        }

    }

    Label {
        visible: isValid(DownloadView.Stages.ComputerSelection)

        text: "Protocol"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    ComboBox {
        visible: isValid(DownloadView.Stages.ComputerSelection)

        property bool loaded: false
        Layout.fillWidth: true
        id: transportSelection
        model: libdivecomputer.transports
        textRole: "description"
        valueRole: "index"

        Component.onCompleted: {
            loaded = true;
        }

        onCurrentIndexChanged: {

            var idx = transportSelection.model.index(transportSelection.currentIndex, 0);
            var value = transportSelection.model.data(idx, TransportRoles.DescriptionRole);
            var transport = transportSelection.model.data(idx, TransportRoles.TransportRole);

            if(loaded && value) {
                session.transportType = value || "";
            }

            var comp_idx = computerSelection.model.index(computerSelection.currentIndex, 0);
            if(idx.valid && comp_idx.valid) {
                var comp = computerSelection.model.data(comp_idx, ComputerRoles.IndexRole);
                libdivecomputer.update_availble_ports(comp, transport);
            }
        }

    }

    Label {
        visible: isValid(DownloadView.Stages.TransportSelection)

        text: "Sources"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    RowLayout {
        visible: isValid(DownloadView.Stages.TransportSelection)

        ComboBox {
            Layout.fillWidth: true
            id: sourceSelection
            model: libdivecomputer.ports
            textRole: "description"
            valueRole: "index"

        }

        Button {
            text: FontAwesome.refresh
            font.family: FontAwesome.fontFamily
            onClicked: {
                var idx = transportSelection.model.index(transportSelection.currentIndex, 0);
                var transport = transportSelection.model.data(idx, TransportRoles.TransportRole);

                var comp_idx = computerSelection.model.index(computerSelection.currentIndex, 0);
                if(idx.valid && comp_idx.valid) {
                    var comp = computerSelection.model.data(comp_idx, ComputerRoles.IndexRole);
                    libdivecomputer.update_availble_ports(comp, transport);
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
        visible: fileRadio.checked
        text: "Output file"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    RowLayout {
        visible: fileRadio.checked
        Layout.fillWidth: true

        TextField {
            Layout.fillWidth: true

            id: filePath
            text: session.path
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
        enabled: isValid(DownloadView.Stages.OutputSelection)

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

        function onTransportChanged() {
            var idx = transportSelection.find(session.transportType);

            transportSelection.loaded = false;

            if(idx > -1) {
                transportSelection.currentIndex = idx;
            } else {
                transportSelection.currentIndex = 0;
            }

            transportSelection.loaded = true;

        }

        function onAvailablePortsChanged() {
            sourceSelection.currentIndex = 0;
        }
    }
}
