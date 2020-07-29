import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import SortFilterProxyModel 0.1
import Libdivecomputer 0.2
import QtQuick.Controls.Material 2.12
import "views"

ApplicationWindow {
    objectName: "appWindow"

    QDCContext {
        id: dccontext
        loglevel: LogLevel.Warning
    }

    Material.theme: Material.Light;
    Material.accent: Material.Blue;

    property int margin: 21
    property int labelColumnWidth: 120
    property int initialWidth: 580
    property int initialHeight: 580

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
    Component {
        id: selectionViewComp
        SelectionWindow {
            id: selectionView
            diveData: selectionProxy.model
        }

    }

    QDCSelectionProxy {
        id: selectionProxy
        onShow: {
            stackView.push(selectionViewComp);
        }
        onHide: {
            stackView.pop();
        }
    }

    Component.onCompleted: {
        if(!littledivelog.isLoggedIn) {
            stackView.push(loginViewComp);
        }
    }

    Connections {
        target: littledivelog
        function onLoggedStateChanged(isLoggedIn) {
            if(isLoggedIn) {
                libdivecomputer.writeType = WriteTypes.LittleLog;
                stackView.pop();
            } else {
                stackView.push(loginViewComp);
            }
        }
        function onRefreshTokenChanged(token) {
            session.refreshToken = token;
        }
    }


}
