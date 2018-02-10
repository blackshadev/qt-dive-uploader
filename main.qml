import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0
import SortFilterProxyModel 0.1
import DCComputer 0.1

ApplicationWindow {
    visible: true
    property int margin: 21
    property int labelColumnWidth: 120
    property int selfRole: ComputerRoles.SelfRole

    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight + 2 * margin
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin
    title: "Dive Uploader"

    FileDialog {
        visible: false
        id: fileDialog
        selectExisting: false
        title: "Please choose a file"
        nameFilters: [ "JSON files (*.json)", "All files (*)" ]
        onAccepted: {
            filePath.text = fileDialog.fileUrl;
            session.path = fileDialog.fileUrl;
            fileDialog.close();
        }
        onRejected: {
            console.log("Canceled")
            fileDialog.close();

        }
    }

    GridLayout{

        id: mainLayout
        anchors.fill: parent
        anchors.margins: margin
        columns: 2

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
            }

            Button {
                text: "Browse..."
                onClicked: {
                    fileDialog.open();
                }
            }
        }

        ProgressBar {
            id: downloadProgress
            Layout.columnSpan: 2
            Layout.fillWidth: true

            Connections {
                target: libdivecomputer
                onProgress: {

                    console.log("HERE");
                    console.log(current);
                    console.log(total);

                }
            }
        }

        Button {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignRight
            text: "Start"
            onClicked: {

                var idx = computerSelection.model.index(computerSelection.currentIndex, 0);
                libdivecomputer.start_download(
                    portSelection.currentText,
                    computerSelection.model.data(idx, selfRole)
                );

            }
        }

        Label {
            text: "DC Version"
        }

        Label {
            Layout.fillWidth: true
            text: libdivecomputer.version
        }

    }
}
