import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import SortFilterProxyModel 0.1
import DCComputer 0.1

ApplicationWindow {
    visible: true
    property int margin: 21
    property int labelColumnWidth: 120

    width: mainView.implicitWidth + 2 * margin
    height: mainView.implicitHeight + 2 * margin
    minimumWidth: mainView.Layout.minimumWidth + 2 * margin
    minimumHeight: mainView.Layout.minimumHeight + 2 * margin
    title: "Dive Uploader"


    MainView {
        id: mainView
    }

    LoginView {
        id: loginView
    }

    StackView {
        id: stackView
        initialItem: mainView
        anchors.fill: parent
        anchors.margins: margin

    }

    Component.onCompleted: {
        stackView.push(loginView)
    }
}
