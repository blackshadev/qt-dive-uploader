import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

RowLayout {

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
        }

        Button {
            id: loginBtn
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignRight
            text: "Login"
            onClicked: {
                stackView.pop();
            }
        }

    }
}
