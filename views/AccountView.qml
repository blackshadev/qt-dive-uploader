import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import FontAwesome 1.0

RowLayout {

    Layout.fillWidth: true
    id: accountLayout

    Loader {
        sourceComponent: littledivelog.userInfo === null ? notLoggedIn : accountData
    }

    Component {
        id: notLoggedIn

        LoginView {
            width: accountLayout.width
            height: accountLayout.height
        }

    }

    Component {
        id: accountData
        ColumnLayout {
            GridLayout {
                Layout.fillWidth: true
                columns: 2

                Label {
                    text: "Name"
                }
                Label {
                    text: littledivelog.userInfo.name
                }

                Label {
                    text: "Email"
                }
                Label {
                    text: littledivelog.userInfo.email
                }

                Label {
                    text: "User since"
                }
                Label {
                    text: littledivelog.userInfo.inserted.toLocaleString(Qt.locale("nl_NL"), "dd-MM-yyyy HH:mm:ss")
                }

                Label {
                    text: "Dive Count"
                }
                Label {
                    text: littledivelog.userInfo.diveCount
                }

                Label {
                    text: "Computer Count"
                }
                Label {
                    text: littledivelog.userInfo.computerCount
                }

                Label {
                    text: "Buddy Count"
                }
                Label {
                    text: littledivelog.userInfo.buddyCount
                }

                Label {
                    text: "Tag Count"
                }
                Label {
                    text: littledivelog.userInfo.tagCount
                }

                RowLayout {
                    Button {
                        text: "Open website"
                        onClicked: {
                            Qt.openUrlExternally("https://dive.littledev.nl/");
                        }
                    }
                    Button {
                        text: "Logout"
                        onClicked: {
                            littledivelog.logout();
                        }
                    }
                }
            }
        }
    }
}
