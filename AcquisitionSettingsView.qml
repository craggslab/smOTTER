import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4

GridLayout {
    property bool acquisitionRunning: false;
    onAcquisitionRunningChanged: {
        if (!acquisitionRunning)
            dataSource.saveNewPhotons();
    }

    columns: 2
    rows: 3

    Label {
        text: qsTr("Experiment Length (mins)")
    }

    SpinBox {
        id: experimentLengthSpinBox
        editable: true

        onValueChanged: dataSource.setExperimentLength(value)

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
                    id: totalDonorPhotonsLabel
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
                    id: totalAcceptorPhotonsLabel
                    text: qsTr("0")
                }
            }

            Label {
                text: qsTr("Experiment Progress:")
            }

            ProgressBar {
                id: experimentProgressBar
                Layout.fillWidth: true

                implicitHeight: 24
                background.height: 24
                contentItem.implicitHeight: 24

                value: refreshProgressBarTimer.timeSoFar / (experimentLengthSpinBox.value * 60000)
            }

            Timer {
                id: refreshProgressBarTimer
                interval: 100.0

                property real timeSoFar: 0.0

                repeat: true

                onTriggered: {
                   acquisitionRunning = dataSource.isRunning()
                    if (acquisitionRunning) {
                        timeSoFar += interval
                        totalDonorPhotonsLabel.text = dataSource.getTotalDonorPhotons();
                        totalAcceptorPhotonsLabel.text = dataSource.getTotalAcceptorPhotons();
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                Button {
                    text: qsTr("START")

                    enabled: !acquisitionRunning;

                    onClicked: {
                        dataSource.startAcquisition()
                        refreshProgressBarTimer.timeSoFar = 0.0
                        refreshProgressBarTimer.start()
                    }
                }

                Button {
                    text: qsTr("STOP")

                    enabled: acquisitionRunning;

                    onClicked: {
                        dataSource.stopAcquisition()
                        refreshProgressBarTimer.stop()
                        refreshProgressBarTimer.timeSoFar = 0.0
                        acquisitionRunning = false
                    }
                }
            }

        }
    }

    function sendValuesToNICard() {
        dataSource.setExperimentLength(experimentLengthSpinBox.value);
    }

    Connections {
        target: dataSource
        onSendValues: sendValuesToNICard();
    }
}

