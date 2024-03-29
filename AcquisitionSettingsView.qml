import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.3
import RadialBar 1.0

ScrollView
{
    id: scrollView
    property int sbWidth: ScrollBar.vertical.width
    clip: true

    property bool acquisitionRunning: false;
    property bool isLive: false
    onAcquisitionRunningChanged: {
        if (!acquisitionRunning && !isLive)
            dataSource.saveNewPhotons(true);
    }

    property alias savePeriodically: saveIntervalCheckBox.checked
    property alias saveInterval: saveIntervalSpinBox.value
    property alias expLength: experimentLengthSpinBox.value
    property alias saveLaserPowers: saveLaserPowersCheckBox.checked

    GridLayout {
        width: Math.max(implicitWidth, scrollView.availableWidth - sbWidth - 10)

        columns: 4

        Label {
            Layout.column: 0
            Layout.row: 0
            text: qsTr("Experiment Length (mins)")
        }

        SpinBox {
            id: experimentLengthSpinBox

            Layout.column: 1
            Layout.row: 0

            editable: true

            onValueChanged: dataSource.setExperimentLength(value)

            enabled: !acquisitionRunning

            value: 60
            from: 1
            to: 1440
        }

        Label {}

        CheckBox {
            id: saveLaserPowersCheckBox

            Layout.column: 0
            Layout.row: 1

            enabled: !acquisitionRunning

            checked: true
            text: qsTr("Save Laser Powers")

            onCheckedChanged: dataSource.setSaveLaserPowers(checked)
        }

        CheckBox {
            id: saveIntervalCheckBox

            Layout.column: 0
            Layout.row: 2

            enabled: !acquisitionRunning

            checked: true
            text: qsTr("Save Interval (mins): ")

        }

        SpinBox {
            id: saveIntervalSpinBox

            editable: true
            value: 1

            enabled: !acquisitionRunning

            from: 1
            to: 5
        }

        Pane {
            id: tmp
            width: nextSaveProgressBar.width
            height: nextSaveProgressBar.height
            Layout.alignment: Qt.AlignCenter

            background: RadialBar {
                anchors.left: parent.left
                anchors.top: parent.top
                id: nextSaveProgressBar

                dialType: RadialBar.FullDial
                startAngle: 180.0
                minValue: 0
                maxValue: saveIntervalSpinBox.value
                value: saveIntervalCheckBox.checked ? (refreshProgressBarTimer.timeSoFar/60000.0) % saveIntervalSpinBox.value : 0
                backgroundColor: "transparent"
                foregroundColor: Material.primary
                progressColor: Material.accent
                showText: false

                dialWidth: 4
                width: 32
                height: 32
            }
        }

        Frame {
            Layout.columnSpan: 4

            Layout.column: 0
            Layout.row: 3

            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout {

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                GridLayout {
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

                Pane {
                    id: experimentProgressBar
                    property alias value: progress.value
                    property int hoursRemaining: 0
                    property int minutesRemaining: 0
                    property int secondsRemaining: 0


                    Layout.fillWidth: true

                    background:  ProgressBar {
                        id: progress

                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        implicitHeight: 24
                        background.height: 24
                        contentItem.implicitHeight: 24

                        value: refreshProgressBarTimer.timeSoFar / (experimentLengthSpinBox.value * 60000)
                    }

                    Label {
                        anchors.fill: parent

                        horizontalAlignment: Text.AlignHCenter

                        text: timeToString(experimentProgressBar.hoursRemaining, experimentProgressBar.minutesRemaining, experimentProgressBar.secondsRemaining)

                        function timeToString(hrs, mins, secs) {
                            if (hrs > 0)
                                return hrs + "h " + mins + "m " + secs + "s"
                            else if (mins > 0)
                                return mins + "m " + secs + "s"
                            else if (secs > 0)
                                return secs + "s"
                            else
                                return ""
                        }
                    }
                }



                Timer {
                    id: refreshProgressBarTimer
                    interval: 100.0

                    property real timeSoFar: 0.0

                    repeat: true

                    onTriggered: {
                        acquisitionRunning = dataSource.isRunning()
                        if (acquisitionRunning) {
                            if (!isLive)
                            {
                                timeSoFar += interval
                                timeSoFarToTimeRemaining(Math.floor(timeSoFar/1000.0))
                            }

                            totalDonorPhotonsLabel.text = dataSource.getTotalDonorPhotons();
                            totalAcceptorPhotonsLabel.text = dataSource.getTotalAcceptorPhotons();
                        }
                    }

                    function timeSoFarToTimeRemaining(timeSoFar) {
                        var secsToGo = experimentLengthSpinBox.value*60 - timeSoFar

                        experimentProgressBar.hoursRemaining = Math.floor(secsToGo/3600)
                        secsToGo = secsToGo % 3600

                        experimentProgressBar.minutesRemaining = Math.floor(secsToGo/60)

                        experimentProgressBar.secondsRemaining = secsToGo % 60
                    }
                }

                Timer {
                    id: saveTimer
                    interval: saveIntervalSpinBox.value * 60000.0
                    repeat: true

                    running: acquisitionRunning && saveIntervalCheckBox.checked && !isLive

                    onTriggered: {
                        dataSource.saveNewPhotons(false);
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignRight

                    Button {
                        text: qsTr("LIVE")

                        enabled: !acquisitionRunning

                        onClicked: {
                            isLive = true
                            dataSource.startAcquisition(true)
                            refreshProgressBarTimer.timeSoFar = 0.0
                            refreshProgressBarTimer.start()
                        }
                    }

                    Button {
                        text: qsTr("START")

                        enabled: !acquisitionRunning

                        onClicked: {
                            if (dataSource.fileExists())
                            {
                                warningTextArea.text = "\n" + dataSource.getFilename() + " already exists\n\nDo you want to overwrite?\n"
                                confirmDialog.open()
                            }
                            else
                            {
                                startSavingAcquisition()
                            }
                        }
                    }

                    Button {
                        text: qsTr("STOP")

                        enabled: acquisitionRunning

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
    }

    function sendValuesToNICard() {
        dataSource.setExperimentLength(experimentLengthSpinBox.value);
    }

    function startSavingAcquisition() {
        isLive = false
        dataSource.startAcquisition()
        refreshProgressBarTimer.timeSoFar = 0.0
        refreshProgressBarTimer.start()
    }

    Connections {
        target: dataSource
        onSendValues: sendValuesToNICard();
    }

    Dialog {
        id: confirmDialog

        Material.theme: Material.Dark
        Material.primary: Material.BlueGrey
        Material.accent: Material.Blue

        parent: mainComponent
        anchors.centerIn: parent

        modal: Qt.WindowModal

        standardButtons: Dialog.No | Dialog.Yes

        title: "Warning:"

        onAccepted: startSavingAcquisition()

        Label {
            id: warningTextArea
            font.pixelSize: 15
        }
    }
}
