import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import SortFilterProxyModel 0.1
import DCComputer 0.1
import Libdivecomputer 0.1
import QtQuick.Controls.Material 2.1

ApplicationWindow {
    objectName: "appWindow"

    Material.theme: Material.Light;
    Material.accent: Material.Blue;


    property int margin: 21
    property int labelColumnWidth: 120
    property int initialWidth: 580
    property int initialHeight: 420
    property int t: DiveModel.DiveDepthRole

    visible: true

    property int _width: mainViewComp.Ready ? mainView.Layout.minimumWidth + 2 * margin : initialWidth
    property int _height: mainViewComp.Ready ? mainView.Layout.minimumHeight + 2 * margin : initialHeight

    id: app
    width: _width
    height: _height
    minimumWidth: _width
    minimumHeight: _height
    title: "Dive Uploader"

    StackView {
        id: stackView
        initialItem: mainViewComp
        anchors.fill: parent
        anchors.margins: margin

        Component {
            id: mainViewComp
            MainView {
                id: mainView
            }
        }

        Component {
            id: loginViewComp
            LoginView {
                id: loginView
            }
        }

    }

    Component.onCompleted: {
        if(!littledivelog.isLoggedIn) {
            stackView.push(loginViewComp);
        }
    }


    Connections {
        target: littledivelog
        onLoggedStateChanged: {
            if(isLoggedIn) {
                stackView.pop();
            } else {
                stackView.push(loginViewComp);
            }
        }
        onRefreshTokenChanged: {
            session.refreshToken = tok;
        }
    }

    Connections {
        target: libdivecomputer
        onSelectDives: {
            selectionwindow.visible = true;
            selectionwindow.setDiveData(writer, dives);
        }
    }


    SelectionWindow {
        width: 480
        height: 520
        visible: false
        id: selectionwindow
        title: "Select Dives"
    }
}
