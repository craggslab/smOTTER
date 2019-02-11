import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4

GridLayout {

    columns: 2
    rows: 3

    Label {
        text: qsTr("Experiment Length (mins)")
    }

    SpinBox {
        editable: true
        value: 60
        from: 1
        to: 1440
    }

    CheckBox {
        checked: true
        text: qsTr("Save Interval (mins): ")
    }

    SpinBox {
        editable: true
        value: 1

        from: 1
        to: 5
    }

    Frame {
        Layout.columnSpan: 2
        Layout.fillHeight: true
        Layout.fillWidth: true

        ColumnLayout {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            GridLayout {
                rows: 2
                columns: 3

                Label {
                    text: qsTr("Total")
                }

                Label {
                    Layout.alignment: Qt.AlignRight
                    text: qsTr("Donor Photons:")
                }

                Label {
                    text: qsTr("0")
                }

                Label {
                    text: qsTr("Total")
                }

                Label {
                    Layout.alignment: Qt.AlignRight
                    text: qsTr("Acceptor Photons:")
                }

                Label {
                    text: qsTr("0")
                }
            }

            Label {
                text: qsTr("Experiment Progress:")
            }

            ProgressBar {
                Layout.fillWidth: true

                implicitHeight: 24
                background.height: 24
                contentItem.implicitHeight: 24

                value: 0.3
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                Button {
                    text: qsTr("START")

                    onClicked: dataSource.startAcquisition()
                }

                Button {
                    text: qsTr("STOP")

                    onClicked: dataSource.stopAcquisition()
                }
            }

        }
    }

}

