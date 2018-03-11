import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3


Item {

    Loader {
        sourceComponent: littledivelog.userInfo === null ? notLoggedIn : accountData
    }

    Component {
        id: notLoggedIn
        ColumnLayout {
            Label {
                text: "Not logged In"
            }
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
                    text: "..."
                }

                Label {
                    text: "Dive Count"
                }
                Label {
                    text: "..."
                }

                Label {
                    text: "Computer Count"
                }
                Label {
                    text: "..."
                }

                Label {
                    text: "Buddy Count"
                }
                Label {
                    text: "..."
                }

                Label {
                    text: "Tag Count"
                }
                Label {
                    text: "..."
                }
            }
        }
    }
}
