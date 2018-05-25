import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import Libdivecomputer 0.1

Window {
    property DiveModel diveData

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
                        text: "#"
                    }

                    Text {
                        text: "Date"
                    }

                    Text {
                        text: "DiveTime"
                    }

                    Text {
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
                    highlighted = selected;
                }


                contentItem: RowLayout {
                    spacing: 10

                    Text {
                        text: index
                    }

                    Text {
                        text: datetime
                    }

                    Text {
                        text: time
                    }

                    Text {
                        text: depth
                    }

                }

            }

        }
    }

}
