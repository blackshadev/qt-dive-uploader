import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

RowLayout {
    Layout.fillWidth: true

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
                    stackView.pop();
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
