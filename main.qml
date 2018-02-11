import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
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



    ColumnLayout {

        id: mainLayout
        anchors.fill: parent
        anchors.margins: margin

        TabBar  {
            id: tabBar
            Layout.fillWidth: true
            TabButton {
                text: qsTr("Downloader")
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
            LogView {
                id: logView
            }
        }

        Connections {
            target: libdivecomputer
            onError: logView.log("ERROR", msg)
            onLog: logView.log(lvl, msg)
        }
    }
}
