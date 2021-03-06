import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import SortFilterProxyModel 0.1
import Libdivecomputer 0.2
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import FontAwesome 1.0
import "../components"

GridLayout {
    property bool isDownloading: dcreader.isBusy
    property bool isWriting: dcwriter.isBusy
    property bool isSelecting: selectionProxy.isBusy
    property bool isBusy: isDownloading || isWriting || isSelecting || littledivelog.isBusy;
    property variant writeTarget: selectDives.checked ? selectionProxy : dcwriter;

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

        startButton.enabled = isValid(DownloadView.Stages.OutputSelection);
    }

    function refreshDevices() {

        var idx = transportSelection.model.index(transportSelection.currentIndex, 0);
        var transport = transportSelection.model.data(idx, TransportRoles.TransportRole);
        var comp_idx = computerSelection.model.index(computerSelection.currentIndex, 0);
        var descriptor = computerSelection.model.data(comp_idx, DescriptorRoles.DescriptorRole);

        if (idx.valid && comp_idx.valid) {
            devicelist.loadDevices(transport, descriptor);
            sourceSelection.currentIndex = 0;
        } else {
            devicelist.clear();
        }
    }

    function ensureJSON(filepath) {
        if(!/\.json$/.test(filepath)) {
            filepath = filepath.replace(/\..*$/, "");
            filepath += ".json";
        }
        return filepath;
    }

    function updateComputerSelection() {
        computerSelection.loaded = false;
        var idx = computerSelection.find(sessionStore.data.computer);
        if(idx > -1) {
            computerSelection.currentIndex = idx;
        }
        computerSelection.loaded = true;
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

    QDCDiveParser {
        id: dcparser
        context: dccontext
    }

    QDCAsyncReader {
        id: dcreader
        parser: dcparser
        context: dccontext
        onProgress: {
            readProgress.value = current / maximum;
            dcwriter.maximum = maximum;
        }
        onDive: {
            writeTarget.write(dive);
        }
        onError: {
            statusLabel.isError = true;
            statusLabel.text = msg;
        }
        onCancelled: {
            writeTarget.cancel();
        }
        onFinished: {
            dcwriter.maximum = divesRead;
            writeTarget.end();
        }
        onDeviceInfo: {
            dcwriter.device = data;
            selectionProxy.device = data;
            const computer = littledivelog.getComputer(data);
            if (computer && useFingerprint.checked) {
                dcreader.setFingerprint(computer.fingerprint);
            } else {
                dcreader.clearFingerprint();
            }

            writeTarget.start();
        }
    }

    QDCWriterController {
        id: dcwriter
        writer: fileRadio.checked ? dcfilewriter : llRadio.checked ? dclittlelogwriter : null;
        onProgress: {
            writeProgress.value = current / maximum;
        }
        onEnded: {
            const usedFingerprint = useFingerprint.checked;
            const divesRead = dcreader.divesRead;
            const divesSkipped = dcreader.maximum - divesRead;
            const divesWritten = dcwriter.current;

            requests.cleanup();
            statusLabel.isError = false;
            statusLabel.text = `Successfully downloaded ${divesRead}; written ${divesWritten}; skipped ${divesSkipped}.`;
        }
    }

    QDCFileExporter {
        id: dcfilewriter
    }

    QDCLittleDiveLogExporter {
        id: dclittlelogwriter
        divelog: littledivelog
    }

    Connections {
        target: sessionStore
        function onLoaded() {
            updateComputerSelection();
            fileRadio.checked = sessionStore.data.writeType === "File";
            llRadio.checked = sessionStore.data.writeType === "Littlelog";
            filePath.text = sessionStore.data.path;
            selectDives.checked = sessionStore.data.selectDives;
            useFingerprint.checked = sessionStore.data.useFingerprint;
        }
    }

    Connections {
        target: selectionProxy
        function onStarted() {
            isSelecting = true;
        }
        function onEnded() {
            var selected = selectionProxy.selected;

            dcwriter.maximum = selected.length;
            dcwriter.start();

            for (let iX = 0; iX < selected.length; iX++) {
                dcwriter.write(selected[iX]);
            }

            dcwriter.end();

            isSelecting = false;
        }
        function onCancelled() {
            isSelecting = false;
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
        enabled: !isBusy

        model: SortFilterProxyModel {
            sourceModel: QDCDescriptorListModel {
                id: descriptorlist
                Component.onCompleted: {
                    descriptorlist.loadDescriptors(dccontext);
                    updateComputerSelection();
                }
            }
            sortRoleName: "description"
            sortOrder: "AscendingOrder"


        }
        textRole: "description"
        onCurrentIndexChanged: {
            var idx = computerSelection.model.index(computerSelection.currentIndex, 0);

            var comp = "";
            var trans = 0;
            var descr;

            if(idx.valid) {
                comp = computerSelection.model.data(idx, DescriptorRoles.DescriptionRole);
                trans = computerSelection.model.data(idx, DescriptorRoles.TransportsRole);
                descr = computerSelection.model.data(idx, DescriptorRoles.DescriptorRole);
            }

            transportlist.filter(trans);
            transportSelection.currentIndex = 0;
            dcwriter.descriptor = descr;
            selectionProxy.descriptor = descr;

            if(loaded) {
                sessionStore.data.computer = comp;
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
        enabled: !isBusy
        model: QDCTransportListModel {
            id: transportlist
            Component.onCompleted: {
                transportlist.loadTransports(dccontext);
                transportSelection.loaded = true;
            }
        }
        textRole: "description"

        onCurrentIndexChanged: {

            var idx = transportSelection.model.index(transportSelection.currentIndex, 0);
            var value = transportSelection.model.data(idx, TransportRoles.DescriptionRole);

            if(loaded && value) {
                sessionStore.data.transportType = value || "";
            }

            refreshDevices();
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
            enabled: !isBusy
            model: QDCDeviceListModel {
                id: devicelist
            }
            textRole: "description"
        }

        Button {
            text: FontAwesome.refresh
            font.family: FontAwesome.fontFamily
            onClicked: {
                refreshDevices();
                refreshUI();
            }
            enabled: !isBusy

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
        enabled: !isBusy
        onCheckStateChanged: {
            sessionStore.data.selectDives = checked;
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
            enabled: !isBusy
            onCheckedChanged: {
                if (checked) {
                    sessionStore.data.writeType = "File";
                }
                refreshUI();
            }
        }

        RadioButton {
            text: "LittleLog"
            id: llRadio
            enabled: !isBusy && littledivelog.userInfo !== null
            onCheckedChanged: {
                if (checked) {
                    sessionStore.data.writeType = "Littlelog";
                }
                refreshUI();
            }
        }
    }

    Label {
        visible: littledivelog.isLoggedIn
        text: "New dives only"
        Layout.minimumWidth: labelColumnWidth
        Layout.maximumWidth: labelColumnWidth

        ToolTip.visible: useFinderprintLabelMouseArea.containsMouse
        ToolTip.timeout: 5000
        ToolTip.text: "Only download new dives based on the previously download dives on your littledev account"
        MouseArea {
            id: useFinderprintLabelMouseArea
            anchors.fill: parent
            hoverEnabled: true
        }
    }

    CheckBox {
        visible: littledivelog.isLoggedIn
        id: useFingerprint
        enabled: !isBusy
        onCheckStateChanged: {
            sessionStore.data.onlyNewDives = checked;
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
            enabled: !isBusy
            onTextChanged: {
                var fixedText = ensureJSON(filePath.text);
                if(filePath.text !== fixedText) {
                    filePath.text = fixedText;
                }

                sessionStore.data.path = fixedText;
                dcfilewriter.path = fixedText;
                refreshUI();
            }
        }

        Button {
            Material.elevation: 0
            text: "Browse..."
            enabled: !isBusy
            onClicked: {
                fileDialog.open();
            }
        }
    }

    LDLProgressBar{
        visible: isBusy
        id: readProgress
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }

    LDLProgressBar {
        visible: isBusy
        id: writeProgress
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }

    Label {
        property bool isError: true
        Layout.fillWidth: true
        Layout.columnSpan: 2
        id: statusLabel
        color: isError ? "red" : Material.color(Material.Blue)
        text: ""
    }

    RoundButton {
        Layout.alignment: Qt.AlignBottom | Qt.AlignRight
        Layout.columnSpan: 2

        id: startButton
        text: FontAwesome.download

        font.family: FontAwesome.fontFamily
        font.pointSize: 25
        padding: 20
        visible: !isDownloading
        enabled: isValid(DownloadView.Stages.OutputSelection)

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

            statusLabel.text = "";
            writeProgress.value = 0;
            readProgress.value = 0;

            var idx = devicelist.index(sourceSelection.currentIndex, 0);
            var dev = devicelist.data(idx, DeviceRoles.DeviceRole);

            dcreader.device = dev;
            dcparser.device = dev;

            dcreader.startReading()
        }
    }

    RoundButton {
        Layout.alignment: Qt.AlignBottom | Qt.AlignRight
        Layout.columnSpan: 2

        id: cancelButton
        text: FontAwesome.ban

        font.family: FontAwesome.fontFamily
        font.pointSize: 25
        padding: 20
        visible: isDownloading

        Material.background: Material.Red
        Material.foreground: Material.color(Material.Grey, Material.Shade100)

        onClicked: {
            dcreader.cancel();
        }
    }

}
