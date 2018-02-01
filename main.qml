import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0

ApplicationWindow {
    visible: true
    property int margin: 11
    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight + 2 * margin
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin
    title: "Dive Uploader"

    FileDialog {
        visible: false
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            filePath.text = fileDialog.fileUrl
            fileDialog.close();
        }
        onRejected: {
            console.log("Canceled")
            fileDialog.close();

        }
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: margin


        RowLayout {
            Label {
                renderType: "NativeRendering"
                text: "Computer"
            }

            ComboBox {
                editable: true
                Layout.fillWidth: true
                model: dc_available_computers

            }
        }
        RowLayout {
            Label {
                renderType: Text.NativeRendering
                text: "TestLabel"
            }

            TextField {
                id: filePath
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
