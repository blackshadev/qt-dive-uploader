import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import Libdivecomputer 0.1

Window {

    Material.theme: Material.Light
    Material.accent: Material.Blue

    property DiveModel diveData
    property bool done: false;
    property variant curWriter;
    property variant columnWidths: {
        "checkbox": 32,
        "datetime": 200,
        "time": 100,
        "depth": 100
    }

    function setDiveData(writer, data) {
        diveData = data;
        curWriter = writer;
        done = false;
    }

    onVisibleChanged: {

        if(!done && diveData !== undefined && visible === false) {
            libdivecomputer.cancel();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {

            z: 3
            Layout.fillWidth: true
            height: 85

            ColumnLayout {

                RowLayout {

                    Button {

                        Layout.leftMargin: 10
                        text: "Select all"
                        Material.elevation: 0
                        onClicked: {
                            diveData.setSelectedAll(true);
                        }
                    }

                    Button {

                        Layout.leftMargin: 10
                        text: "Deselect all"
                        Material.elevation: 0
                        onClicked: {
                            diveData.setSelectedAll(false);
                        }
                    }

                    Button {

                        Layout.leftMargin: 10
                        text: "Done"
                        enabled: curWriter !== undefined
                        Material.background: Material.color(Material.Blue)
                        Material.theme: Material.Dark
                        onClicked: {
                            curWriter.selectionDone();
                            done = true;
                            selectionwindow.visible = false;
                        }
                    }
                }

                Text {
                    Layout.leftMargin: 10
                    Layout.topMargin: 5
                    text: diveData ? "Selected " + diveData.selectedDiveCount + " / " + diveData.rowCount() : "No dive data"
                }
            }
        }

        ListView {

            id: listView
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            Layout.fillHeight: true
            Layout.fillWidth: true

            ScrollBar.vertical: ScrollBar {}

            model: diveData

            headerPositioning: ListView.OverlayHeader
            header: Rectangle {
                width: parent.width
                height: 32

                z: 2

                CustomBorder {
                    commonBorder: false
                    bBorderwidth: 1
                    borderColor: Material.color(Material.Grey)
                }

                RowLayout {
                    anchors.fill: parent

                    Text {
                        Layout.margins: 4
                        Layout.preferredWidth: columnWidths['datetime'] + columnWidths["checkbox"]
                        text: "Date"
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 32
                    }

                    Text {
                        Layout.margins: 4
                        Layout.preferredWidth: columnWidths['time']
                        text: "DiveTime"
                        horizontalAlignment: Text.AlignLeft
                    }

                    Text {
                        Layout.margins: 4
                        Layout.preferredWidth: columnWidths['depth']
                        text: "Depth"
                        horizontalAlignment: Text.AlignLeft
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                }
            }

            delegate: ItemDelegate {
                width: parent.width
//                height: 32
                topPadding: 8

                checkable: true
                highlighted: selected

                Binding {
                    target: background
                    property: "color"
                    value: selected ? Material.color(Material.Blue, Material.Shade50) :  Material.color(Material.Grey, Material.Shade50)
                }


                onClicked: {
                    diveData.setSelected(index, !selected);
                }

                contentItem: RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    CheckBox {
                        padding: 0;
                        Layout.margins: 0
                        checked: selected
                        onCheckedChanged: {
                            diveData.setSelected(index, checked);
                        }
                        Layout.preferredWidth: columnWidths['checkbox']

                    }

                    Text {
                        Layout.preferredWidth: columnWidths['datetime']
                        text: datetime
                        horizontalAlignment: Text.AlignLeft
                    }

                    Text {
                        Layout.preferredWidth: columnWidths['time']
                        text: time
                        horizontalAlignment: Text.AlignLeft
                    }

                    Text {
                        Layout.preferredWidth: columnWidths['depth']
                        text: depth
                        horizontalAlignment: Text.AlignRight
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                }

            }

        }
    }

}
