import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import ByteImage 1.0
import QtQuick.Controls.Material 2.3

Item {

    RowLayout {
        anchors.fill: parent

        Pane {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ByteImage {
                id: imgDisp
                width: Math.min(parent.width, parent.height * aspectRatio)
                height: Math.min(parent.height, parent.width / aspectRatio)
            }
        }

        Pane {
            Layout.fillWidth: false
            Layout.fillHeight: true

            GridLayout {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right

                columns: 4

                Label {
                    text: "Camera ID "
                    horizontalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    Material.theme: Material.Light
                    Layout.fillWidth: true

                    Layout.alignment: Qt.AlignRight

                    model: thorcam.availableCameras

                    onCurrentTextChanged: {
                        if (currentText != "")
                            thorcam.idChanged(currentText);
                    }
                }

                Button {
                    Layout.fillWidth: false

                    Layout.alignment: Qt.AlignRight

                    text: "Start"

                    onClicked: {
                        var success;
                        if (text === "Start") {
                            success = thorcam.start();
                            if (success)
                            {
                                text = "Stop";
                                cameraSnapTimer.start();
                            }
                        } else {
                            success = thorcam.stop();
                            if (success)
                            {
                                text = "Start";
                                cameraSnapTimer.stop();
                            }
                        }
                    }
                }

                Button {
                    Layout.fillWidth: false
                    Layout.alignment: Qt.AlignRight

                    text: "⟳"

                    onClicked: thorcam.refreshCameras()
                }

                Label {
                    Layout.fillWidth: false
                    Layout.alignment: Qt.AlignVCenter

                    text: "Exposure"
                    verticalAlignment: Qt.AlignVCenter
                }

                Slider {
                    id: exposureSlider
                    Layout.fillWidth: true

                    from: thorcam.exposureMin
                    to: thorcam.exposureMax
                    value: thorcam.exposure

                    onPressedChanged: {
                        if (!pressed) thorcam.exposure = value;
                    }

                    Layout.columnSpan: 2
                }

                Label {
                    Layout.fillWidth: false

                    text: exposureSlider.value.toFixed(2) + "ms"
                    verticalAlignment: Text.AlignVCenter
                }

                Label {
                    text: "Z-Stage COM Port"
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: cmbxCOMPort
                    Layout.fillWidth: true
                    Material.theme: Material.Light

                    model: zStage.availableCOMPorts
                }

                Button {
                    text: "Connect"

                    onClicked: zStage.connect(cmbxCOMPort.currentText)
                }

                Button {
                    Layout.fillWidth: false
                    Layout.alignment: Qt.AlignRight

                    text: "⟳"

                    onClicked: zStage.refreshCOMPorts()
                }

                Label {
                    text: "Z Position"
                    verticalAlignment: Text.AlignVCenter
                }

                Slider {
                    id: zPosSlider
                    Layout.fillWidth: true

                    Layout.columnSpan: 2

                    from: 0
                    to: 300
                    value: zStage.zPos

                    onPressedChanged: {
                        if (!pressed) zStage.zPos = value;
                    }
                }

                Label {
                    text: zPosSlider.value.toFixed(0) + "μm"
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    Timer {
        id: cameraSnapTimer
        interval: 200
        repeat: true

        onTriggered: thorcam.snap(imgDisp);
    }

    Connections {
        target: zStage
        onError: {
            errorTextArea.text = errMsg;
            confirmDialog.visible = true
        }
    }

    Dialog {
        id: confirmDialog

        Material.theme: Material.Dark
        Material.primary: Material.BlueGrey
        Material.accent: Material.Blue

        parent: mainComponent
        anchors.centerIn: parent

        modal: Qt.WindowModal

        standardButtons: Dialog.Ok

        title: "Error:"

        Label {
            id: errorTextArea
            font.pixelSize: 15
        }
    }
}
