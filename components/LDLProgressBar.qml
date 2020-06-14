import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12

ProgressBar {
    id: control
    property color backgroundColor: Material.color(Material.Grey, Material.Shade300)
    property color progressColor: Material.color(Material.Blue, Material.Shade300)
    property int radius: height

    height: 12

    background: Rectangle {
        implicitHeight: parent.height
        implicitWidth: parent.width
        radius: control.radius
        color: backgroundColor
    }

    contentItem: Item {
        implicitHeight: parent.height
        implicitWidth: parent.width

        Rectangle {
            width: control.visualPosition * parent.width
            height: parent.height

            radius: control.radius
            color: progressColor
        }
    }
}
