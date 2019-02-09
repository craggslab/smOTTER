import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

ScrollView {
    id: scrollView

    property int sbWidth: ScrollBar.vertical.width
    ScrollBar.vertical.policy: ScrollBar.AlwaysOn

    clip: true

    GridLayout {
        width: Math.max(implicitWidth, scrollView.availableWidth - sbWidth - 10)
        id: contentGrid
        rows: 9
        columns: 2

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            Layout.columnSpan: 2

            TextField {
                id: folderTextField
                Layout.fillWidth: true
                placeholderText: "Save Directory"
            }

            Button {
                text: qsTr("...")
                onClicked: folderDialog.open()
            }

            FileDialog {
                id: folderDialog
                title: qsTr("Choose a folder")
                folder: shortcuts.home
                onAccepted: {
                    folderTextField.text = fileUrl
                }
            }
        }

        TextField {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter

            Layout.columnSpan: 2
            placeholderText: "Filename"
        }

        Label {
            Layout.columnSpan: 2
            text: qsTr("Sample Details")
        }

        TextArea {
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Donor Label")
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Acceptor Label")
        }

        TextField {
            Layout.fillWidth: true
        }

        TextField {
            Layout.fillWidth: true
        }

        Label {
            Layout.columnSpan: 2
            text: qsTr("Buffer")
        }

        TextField {
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Donor Laser Power")
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Acceptor Laser Power")
        }

        TextField {
            Layout.fillWidth: true
        }

        TextField {
            Layout.fillWidth: true
        }

        Label {
            Layout.columnSpan: 2
            text: qsTr("Notes")
        }

        TextArea {
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        Label {
            Layout.columnSpan: 2
            text: qsTr("User(s)")
        }

        TextField {
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }
    }

}
