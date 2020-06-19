import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import SortFilterProxyModel 0.1
import DCComputer 0.1
import Libdivecomputer 0.1
import QtQuick.Controls.Material 2.12
import "views"

ApplicationWindow {
    objectName: "appWindow"

    Material.theme: Material.Light;
    Material.accent: Material.Blue;

    property int margin: 21
    property int labelColumnWidth: 120
    property int initialWidth: 580
    property int initialHeight: 490
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

    Connections {
        target: libdivecomputer
        function onSelectDives(writer, dives) {

            function finishCreation() {
                if (incubator.status === Component.Ready) {
                    var selectionWindow = incubator.object;
                    selectionWindow.onCancelled.connect(function() {
                        stackView.pop();
                        libdivecomputer.cancel();
                    });
                    selectionWindow.onFinished.connect(function() {
                        writer.selectionDone();
                        stackView.pop();
                    });
                    stackView.push(selectionWindow);
                }
            }

            var component = Qt.createComponent("SelectionWindow.qml");
            var incubator = component.incubateObject(stackView, {
                diveData: dives
            });

            if(incubator.status === Component.Ready || incubator.status === Component.Error) {
                finishCreation();
            } else {
                incubator.onStatusChanged = finishCreation;
            }

        }
    }

}
