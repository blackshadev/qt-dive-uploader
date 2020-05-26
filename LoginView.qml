import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

ColumnLayout {

    Layout.fillWidth: true

    Text {
        Layout.fillWidth: true
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 17
        text: "Welcome to the Little Dive Uploader."
    }

    Text {
        Layout.fillWidth: true
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 12
        text: "You can log into your little divelog account or use the tool offline.\nDon't have an account yet? Press the register button below."
    }

    GridLayout {

        Layout.fillWidth: true
        columns: 2




        Label {
            text: "E-Mail"
            Layout.minimumWidth: labelColumnWidth
            Layout.maximumWidth: labelColumnWidth
        }

        TextField {
            id: email
            Layout.fillWidth: true
            Keys.onReturnPressed: {
                password.focus = true;
            }
        }

        Label {
            text: "Password"
            Layout.minimumWidth: labelColumnWidth
            Layout.maximumWidth: labelColumnWidth
        }

        TextField {
            id: password
            echoMode: TextInput.Password
            Layout.fillWidth: true
            Keys.onReturnPressed: {
                littledivelog.login(email.text, password.text);
            }
        }

        Label {
            Layout.columnSpan: 2
            id: errorLbl
            color: "red"
        }

        RowLayout {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignRight

            Button {
                visible: stackView.depth > 1
                text: "Offline"
                Material.elevation: 0
                onClicked: {
                    libdivecomputer.writeType = WriteTypes.File;
                    stackView.pop();
                }
            }

            Button {
                visible: stackView.depth > 1
                text: "Register"
                Material.elevation: 0
                onClicked: {
                    Qt.openUrlExternally("https://dive.littledev.nl/register");
                }
            }

            Button {
                id: loginBtn
                text: "Login"
                Material.background: Material.color(Material.Blue)
                Material.theme: Material.Dark

                onClicked: {
                    littledivelog.login(email.text, password.text);
                }
            }
        }



    }
    Connections {
        target: littledivelog
        onError: {
            errorLbl.text = msg;
        }
    }
}
