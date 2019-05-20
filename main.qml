import QtQuick 2.12
import QtQuick.Window 2.12
import QtCharts 2.3
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.3
import QtQuick.Controls.Universal 2.3
import Qt.labs.settings 1.1


Pane {
    id: mainWindow
    width: 1560
    height: 900


    Material.theme: Material.Dark
    Material.primary: Material.BlueGrey
    Material.accent: Material.Blue

    Universal.theme: Universal.Dark
    Universal.accent: Universal.Cyan



    Settings {
        category: "General"
        property alias mainWindow_x: mainWindow.x
        property alias mainWindow_y: mainWindow.y
        property alias mainWindow_width: mainWindow.width
        property alias mainWindow_height: mainWindow.height

        fileName: "./smfBox_Settings.ini"
    }

    Settings {
        category: "SaveSettings"
        property alias donorLabel: saveSettings.donorLabel
        property alias acceptorLabel: saveSettings.acceptorLabel
        property alias donorExWavelength: saveSettings.donorExWavelength
        property alias acceptorExWavelength: saveSettings.acceptorExWavelength
        property alias donorDetWavelength: saveSettings.donorDetWavelength
        property alias acceptorDetWavelength: saveSettings.acceptorDetWavelength
        property alias donorLaserPower: saveSettings.donorLaserPower
        property alias acceptorLaserPower: saveSettings.acceptorLaserPower
        property alias userName: saveSettings.userName
        property alias userAffiliation: saveSettings.userAffiliation

        fileName: "./smfBox_Settings.ini"
    }

    Settings {
        category: "AcquisitionSettings"
        property alias savePeriodically: acquisitionSettings.savePeriodically
        property alias saveInterval: acquisitionSettings.saveInterval
        property alias expLength: acquisitionSettings.expLength

        fileName: "./smfBox_Settings.ini"
    }

    Settings {
        category: "LaserSettings"
        property alias donorOffPercent: laserDutyCycles.donorOffPercent
        property alias donorOnPercent: laserDutyCycles.donorOnPercent
        property alias acceptorOffPercent: laserDutyCycles.acceptorOffPercent
        property alias acceptorOnPercent: laserDutyCycles.acceptorOnPercent
        property alias alexPeriod: laserDutyCycles.alexPeriod

        fileName: "./smfBox_Settings.ini"
    }

    Settings {
        category: "NISettings"
        property alias deviceDefault: niSettings.deviceDefault
        property alias donorLaserPinDefault: niSettings.donorLaserPinDefault
        property alias acceptorLaserPinDefault: niSettings.acceptorLaserPinDefault
        property alias donorDetectorCounterDefualt: niSettings.donorDetectorCounterDefualt
        property alias acceptorDetectorCounterDefault: niSettings.acceptorDetectorCounterDefault
        property alias donorDetectorPinDefault: niSettings.donorDetectorPinDefault
        property alias acceptorDetectorPinDefault: niSettings.acceptorDetectorPinDefault
        property alias donorDetectorGateDefault: niSettings.donorDetectorGateDefault
        property alias acceptorDetectorGateDefault: niSettings.acceptorDetectorGateDefault
        property alias laserPowerDetectorPin: niSettings.laserPowerPinDefault
        property alias timebaseDefault: niSettings.timebaseDefault
        property alias laserControlResolution: niSettings.laserControlResolution
        property alias timestampAdjustmentValue: niSettings.timestampAdjustmentValue

        fileName: "./smfBox_Settings.ini"
    }

    Settings {
        category: "ESHistogramSettings"
        property alias numBins: esHistogram.nBins

        fileName: "./smfBox_Settings.ini"
    }

    GridLayout {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        rowSpacing: 10

        rows: 2
        columns: 3

        GroupBox {
            Layout.alignment: Qt.AlignTop
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            Layout.preferredHeight: 1.5

            Material.elevation: 6

            label: TabBar {
                id: tabBar
                Layout.fillWidth: true
                Layout.row: 0

                TabButton {
                    text: qsTr("Live Trace")
                    width: implicitWidth
                }
                TabButton {
                    text: qsTr("Photon Distribution")
                    width: implicitWidth
                }
                TabButton {
                    text: qsTr("NI Card Settings")
                    width: implicitWidth
                }
            }

            StackLayout {
                anchors.fill: parent

                currentIndex: tabBar.currentIndex
                Layout.row: 1

                Page {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    LiveTraceView {
                        id: traceView
                        width: parent.width
                        height: parent.height

                        acquisitionRunning: acquisitionSettings.acquisitionRunning
                    }
                }

                Page {
                    TextArea {
                        width: parent.width
                        height: parent.height

                        text: "Coming soon...."
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter

                        enabled: false
                    }
                }

                Pane {
                    NISettingsView {
                        id: niSettings
                        width: parent.width
                        height: parent.height

                        enabled: !acquisitionSettings.acquisitionRunning
                    }
                }

            }

        }

        GroupBox {
            title: "ES Histogram"

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1
            Layout.preferredHeight: 1

            Material.elevation: 6

            ESHistogramView {
                id: esHistogram
                anchors.fill: parent

                acquisitionRunning: acquisitionSettings.acquisitionRunning
            }
        }

        GroupBox {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1
            Layout.preferredHeight: 1


            title: "Acquisition"

            Material.elevation: 6

            AcquisitionSettingsView {
                id: acquisitionSettings
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
            }
        }

        GroupBox {
            title: "Laser Duty Cycles"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1.2
            Layout.preferredHeight: 1

            Material.elevation: 6

            LaserDutyCyclesView {
                id: laserDutyCycles
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                enabled: !acquisitionSettings.acquisitionRunning
            }
        }

        GroupBox {
            title: "Save Settings"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1.2
            Layout.preferredHeight: 1

            Material.elevation: 6

            SaveSettingsView {
                id: saveSettings
                width: parent.width
                height: parent.height

                enabled: !acquisitionSettings.acquisitionRunning
            }

        }

    }

    Timer {
        id: refreshTimer
        interval: 20
        running: acquisitionSettings.acquisitionRunning
        repeat: true

        onTriggered: {
            traceView.updateDisplay()
            esHistogram.updateDisplay()
        }

        onRunningChanged: {
            if (running)
            {
                traceView.resetDisplay()
                esHistogram.resetDisplay()
            }
        }
    }

    Popup {
        id: errorMessageDialog

        Material.theme: Material.Dark
        Material.primary: Material.BlueGrey
        Material.accent: Material.Blue

        anchors.centerIn: parent

        modal: Qt.WindowModal

        padding: 20

        ColumnLayout {
            anchors.fill: parent

            Label {
                font.pixelSize: 20
                text: "Error: "
            }

            TextArea {
                Layout.alignment: Qt.AlignCenter
                id: errorTextArea
                readOnly: true
                selectByMouse: true
            }
        }
    }




    Connections {
        target: dataSource
        onError: {
            errorTextArea.text = msg
            errorMessageDialog.visible = true
        }
    }

}
