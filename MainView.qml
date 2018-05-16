import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

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
    }

}
