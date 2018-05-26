import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import Libdivecomputer 0.1
import "./ColumnHelper.js" as ColumnHelper

Window {
    property DiveModel diveData
    function setDiveData(data) {
        diveData = data;
        listView.columnWidths = ColumnHelper.calcColumnWidths(diveData, listView);
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {

            Button {
                text: "Select all"
                onClicked: {
                    diveData.setSelectedAll(true);
                }
            }

            Button {
                text: "Deselect all"
                onClicked: {
                    diveData.setSelectedAll(false);
                }
            }


        }

        ListView {

            id: listView
            property variant columnWidths: ({});
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
                    spacing: 10

                    Text {
                        width: listView.columnWidths['datetime']
                        text: "Date"
                    }

                    Text {
                        width: listView.columnWidths['time']
                        text: "DiveTime"
                    }

                    Text {
                        width: listView.columnWidths['depth']
                        text: "Depth"
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
                    spacing: 10

                    Text {
                        width: listView.columnWidths['datetime']
                        text: datetime
                    }

                    Text {
                        width: listView.columnWidths['time']
                        text: time
                    }

                    Text {

                        width: listView.columnWidths['depth']
                        text: depth
                    }

                }

            }

        }
    }

}
