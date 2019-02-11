import QtQuick 2.12
import QtQuick.Window 2.12
import QtCharts 2.3
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Controls.Universal 2.3
import Qt.labs.settings 1.1


Pane {
    id: mainWindow
    width: 1440
    height: 900


    Material.theme: Material.Dark
    Material.primary: Material.BlueGrey
    Material.accent: Material.Blue

    Universal.theme: Universal.Dark
    Universal.accent: Universal.Cyan

    Settings { //TODO: finish persistent settings
        property alias mainWindow_x: mainWindow.x
        property alias mainWindow_y: mainWindow.y
        property alias mainWindow_width: mainWindow.width
        property alias mainWindow_height: mainWindow.height

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
                        width: parent.width
                        height: parent.height
                    }
                }

                Page {
                    TextArea {
                        width: parent.width
                        height: parent.height

                        text: "Coming soon...."
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                }

                Pane {
                    NISettingsView {
                        id: niSettings
                        width: parent.width
                        height: parent.height
                    }
                }

            }

        }




        GroupBox {
            id: histoBox
            title: "ES Histogram"

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1
            Layout.preferredHeight: 1

            Material.elevation: 6
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
            Layout.preferredWidth: 1
            Layout.preferredHeight: 1

            Material.elevation: 6

            LaserDutyCyclesView {
                id: laserDutyCycles
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
            }
        }

        GroupBox {
            title: "Save Settings"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1
            Layout.preferredHeight: 1

            Material.elevation: 6

            SaveSettingsView {
                id: saveSettings
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
            }

        }

    }

}
