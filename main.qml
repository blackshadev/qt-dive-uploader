import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0

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

        ColumnLayout {

            RowLayout {
                Label {
                    renderType: "NativeRendering"
                    text: "Computer"
                }

                ComboBox {
                    property bool loaded: false
                    id: computerSelection
                    Layout.fillWidth: true
                    editable: true
                    model: dc_available_computers
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
                Label {
                    renderType: Text.NativeRendering
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
                Label {
                    renderType: Text.NativeRendering
                    text: "DC Version"
                }
                Label {
                    renderType: Text.NativeRendering
                    text: dc_version
                }
            }
        }
    }
}
