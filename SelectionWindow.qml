import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import Libdivecomputer 0.1

Window {
    property DiveModel diveData
    property variant curWriter;
    property variant columnWidths: {
        "datetime": 200,
        "time": 100,
        "depth": 100
    }

    function setDiveData(writer, data) {
        diveData = data;
        curWriter = writer;
    }

    onVisibleChanged: {
        if(diveData !== undefined && visible === false) {
            console.log("cancel");
            libdivecomputer.cancel();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {

            z: 3
            color: "#ffffff"
            Layout.fillWidth: true
            height: 45

            RowLayout {

                Button {

                    Layout.margins: 10
                    text: "Select all"
                    onClicked: {
                        diveData.setSelectedAll(true);
                    }
                }

                Button {

                    Layout.margins: 10
                    text: "Deselect all"
                    onClicked: {
                        diveData.setSelectedAll(false);
                    }
                }

                Button {

                    Layout.margins: 10
                    text: "Done"
                    enabled: curWriter !== undefined
                    onClicked: {
                        curWriter.selectionDone();
                        visible = false;
                    }
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
                color: "#cccccc"
                RowLayout {
                    anchors.fill: parent

                    Text {
                        Layout.margins: 4
                        Layout.preferredWidth: columnWidths['datetime']
                        text: "Date"
                        horizontalAlignment: Text.AlignLeft
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
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                }
            }

            delegate: ItemDelegate {
                width: parent.width
                height: 32

                checkable: true
                highlighted: selected
                onClicked: {
                    diveData.setSelected(index, !selected);
                }


                contentItem: RowLayout {
                    Layout.fillWidth: true

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
