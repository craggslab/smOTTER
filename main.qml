import QtQuick 2.12
import QtQuick.Window 2.12
import QtCharts 2.3
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

Pane {
    width: 640
    height: 480

    //Universal.theme: Universal.Dark
    //Universal.accent: Universal.Steel
    Material.theme: Material.Dark
    Material.primary: Material.BlueGrey
    Material.accent: Material.Red


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
            Layout.preferredHeight: 2

            Material.elevation: 6

            label: TabBar {
                id: tabBar
                Layout.fillWidth: true
                Layout.row: 0

                TabButton {
                    text: qsTr("Tab1")
                    width: implicitWidth
                }
                TabButton {
                    text: qsTr("Tab2")
                    width: implicitWidth
                }
                TabButton {
                    text: qsTr("Tab3")
                    width: implicitWidth
                }
            }

            StackLayout {
                anchors.fill: parent

                currentIndex: tabBar.currentIndex
                Layout.row: 1

                Page {
                    TextArea {
                        width: parent.width
                        height: parent.height

                        text: "Page 1"
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                }

                Page {
                    TextArea {
                        width: parent.width
                        height: parent.height

                        text: "Comming Sonn...."
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                }

                Page {
                    TextArea {
                        width: parent.width
                        height: parent.height

                        text: "Coming soon..."
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                }

            }

        }




        GroupBox {
            id: histoBox
            title: "Histogram"

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1
            Layout.preferredHeight: 1

            Material.elevation: 6
        }

        GroupBox {
            title: "Waveform Settings"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1
            Layout.preferredHeight: 1

            Material.elevation: 6


        }

        GroupBox {
            title: "Save Location"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1
            Layout.preferredHeight: 1

            Material.elevation: 6
        }

        GroupBox {
            title: "Space"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: 1
            Layout.preferredHeight: 1

            Material.elevation: 6
        }

    }

}
