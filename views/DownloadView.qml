import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import SortFilterProxyModel 0.1
import DCComputer 0.1
import DCTransport 0.1
import Libdivecomputer 0.2
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import FontAwesome 1.0
import "../components"

GridLayout {
    property bool isDownloading: false

    Layout.fillWidth: true
    columns: 2
    id: downloadLayout

    enum Stages {
        None = 0,
        ComputerSelection = 1,
        TransportSelection = 2,
        SourceSelection = 3,
        OutputSelection = 4
    }

    function isValid(stage) {
        var idx;
        var validStages = 1 << DownloadView.Stages.None;
        var idx;

        idx = computerSelection.model.index(computerSelection.currentIndex, 0);
        if(idx.valid) {
            validStages |= 1 << DownloadView.Stages.ComputerSelection;
        }

        idx = transportSelection.model.index(transportSelection.currentIndex, 0);
        if(idx.valid) {
            validStages |= 1 << DownloadView.Stages.TransportSelection;
        }

        idx = sourceSelection.model.index(sourceSelection.currentIndex, 0);
        if(idx.valid) {
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

    function refreshUI() {
        transportSelection.visible = isValid(DownloadView.Stages.ComputerSelection);
        transportSelectionLabel.visible = isValid(DownloadView.Stages.ComputerSelection);

        sourceLabel.visible = isValid(DownloadView.Stages.TransportSelection);
        sourceRow.visible = isValid(DownloadView.Stages.TransportSelection);

        startButton.enabled = isValid(DownloadView.Stages.OutputSelection) && isDownloading == false;
    }

    function ensureJSON(filepath) {
        if(!/\.json$/.test(filepath)) {
            filepath = filepath.replace(/\..*$/, "");
            filepath += ".json";
        }
        return filepath;
    }

    FileDialog {
        visible: false
        id: fileDialog
        selectExisting: false
        folder: shortcuts.home
        title: "Please choose a file to save"
        nameFilters: [ "JSON files (*.json)", "All files (*)" ]
        onAccepted: {
            var text = fileDialog.fileUrl.toString();
            var cleanupRegExp = /^(file:\/{2})/;
            // windows specific
            if(/^file:\/\/\/[A-Z]\:/.test(text)) {
                cleanupRegExp = /^(file:\/{3})/;
            }

            text = text.replace(cleanupRegExp, "");
            text = decodeURIComponent(text);
            text = ensureJSON(text);

            filePath.text = text;

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
            sourceModel: divecomputer.descriptors
            sortRoleName: "description"
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

            var idx = computerSelection.model.index(computerSelection.currentIndex, 0);

            var comp = "";
            var trans = 0;

            if(idx.valid) {
                comp = computerSelection.model.data(idx, ComputerRoles.DescriptionRole);
                trans = computerSelection.model.data(idx, ComputerRoles.TransportsRole);
            }

            divecomputer.transports.filter(trans);

            if(loaded) {
                session.computer = comp;
            }

            refreshUI();
        }
    }

    Label {
        visible: isValid(DownloadView.Stages.ComputerSelection)

        text: "Protocol"
        id: transportSelectionLabel
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    ComboBox {
        visible: isValid(DownloadView.Stages.ComputerSelection)

        property bool loaded: false
        Layout.fillWidth: true
        id: transportSelection
        model: divecomputer.transports
        textRole: "description"

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

            }

            refreshUI();
        }

    }

    Label {
        visible: isValid(DownloadView.Stages.TransportSelection)

        id: sourceLabel
        text: "Sources"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    RowLayout {
        visible: isValid(DownloadView.Stages.TransportSelection)

        id: sourceRow
        ComboBox {
            Layout.fillWidth: true
            id: sourceSelection
            model: QDCDeviceListModel {
                id: devicelist
            }
            textRole: "description"
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

            hoverEnabled: true
            ToolTip.timeout: 5000
            ToolTip.text: "Refresh available source ports"
            ToolTip.visible: hovered

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
        visible: littledivelog.isLoggedIn
        text: "Only download new dives"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth
    }

    CheckBox {
        visible: littledivelog.isLoggedIn
        id: useFingerprint
        Component.onCompleted: {
            checked = session.useFingerprint;
        }
        onCheckStateChanged: {
            session.onlyNewDives = checked;
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
                refreshUI();
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


    LDLProgressBar{
        visible: isDownloading
        id: readProgress
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }

    LDLProgressBar {
        visible: isDownloading
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
        Layout.alignment: Qt.AlignBottom | Qt.AlignRight

        id: startButton
        text: FontAwesome.download

        font.family: FontAwesome.fontFamily
        font.pointSize: 25
        padding: 20
        enabled: isValid(DownloadView.Stages.OutputSelection)
        visible: isDownloading == false

        Material.foreground: Material.color(Material.Grey, Material.Shade100)

        hoverEnabled: true
        ToolTip.timeout: 5000
        ToolTip.text: "Start download"
        ToolTip.visible: hovered

        onEnabledChanged: {
            startButton.background.color = enabled ? Material.color(Material.Blue) : Material.color(Material.BlueGrey);
        }

        Component.onCompleted: {
            startButton.background.color = enabled ? Material.color(Material.Blue) : Material.color(Material.BlueGrey);
        }

        onClicked: {
            if(!isValid(DownloadView.Stages.OutputSelection)) {
                refreshUI();
                return;
            }

            errorLabel.text = "";
            writeProgress.value = 0;
            readProgress.value = 0;

            var comp_idx = computerSelection.model.index(computerSelection.currentIndex, 0);
            var port_idx = sourceSelection.model.index(sourceSelection.currentIndex, 0);

            libdivecomputer.start_download(
                sourceSelection.model.data(port_idx, ComputerRoles.IndexRole),
                computerSelection.model.data(comp_idx, ComputerRoles.IndexRole),
                selectDives.checked,
                useFingerprint.checked && littledivelog.isLoggedIn
            );

        }
    }

    RoundButton {
        Layout.alignment: Qt.AlignBottom | Qt.AlignRight

        id: cancelButton
        text: FontAwesome.ban

        font.family: FontAwesome.fontFamily
        font.pointSize: 25
        padding: 20
        visible: isDownloading == true

        Material.background: Material.Red
        Material.foreground: Material.color(Material.Grey, Material.Shade100)

        onClicked: {
            libdivecomputer.cancel();
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
            isDownloading = true;
            refreshUI();
        }

        function onFinished() {
            isDownloading = false;
            refreshUI();
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

            refreshUI();
        }

        function onAvailablePortsChanged() {
            sourceSelection.currentIndex = 0;

            refreshUI();
        }
    }
}
