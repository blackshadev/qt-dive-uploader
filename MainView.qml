import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

ColumnLayout {

    TabBar  {
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
            text: qsTr("Grid")
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
        GridComponent {}
    }

}
