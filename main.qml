import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0
import SortFilterProxyModel 0.1

ApplicationWindow {
    visible: true
    property int margin: 21
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

    ColumnLayout{

        id: mainLayout
        anchors.fill: parent
        anchors.margins: margin


        RowLayout {

            Layout.fillWidth: true

            Label {
                text: "Port"
            }

            ComboBox {
                property bool loaded: false
                id: portSelection
                Layout.fillWidth: true
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
        }

        RowLayout {

            Layout.fillWidth: true

            Label {
                text: "Computer"
            }

            ComboBox {
                property bool loaded: false
                id: computerSelection
                Layout.fillWidth: true
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
        }


        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Output file"
            }

            TextField {
                id: filePath
                text: session.path
                readOnly: true
                Layout.fillWidth: true
            }

            Button {
                text: "Browse..."
                onClicked: {
                    fileDialog.open();
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight
            Button {
                text: "Start"
                onClicked: {
                    libdivecomputer.start_download()
                }
            }
        }

        RowLayout {

            Layout.fillWidth: true

            Label {
                text: "DC Version"
            }
            Label {
                text: libdivecomputer.version
            }
        }

    }
}
