import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {

    TabBar  {
        z: 50
        id: tabBar
        Layout.fillWidth: true
        TabButton {
            text: qsTr("Downloader")
        }

        TabButton {
            text: qsTr("Account")
        }

        TabButton {
            text: qsTr("Log")
        }

        TabButton {
            text: qsTr("About")
        }

    }

    StackLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: tabBar.currentIndex

        DownloadView {}
        AccountView {}
        LogView {
            id: logView
        }
        AboutView {}
    }

}
