import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3

ScrollView {
    id: scrollView

    property int sbWidth: ScrollBar.vertical.width
    ScrollBar.vertical.policy: ScrollBar.AlwaysOn
    clip: true

    property alias deviceDefault: deviceComboBox.defaultStr
    property alias donorLaserPinDefault: donorLaserPinComboBox.defaultStr
    property alias acceptorLaserPinDefault: acceptorLaserPinComboBox.defaultStr
    property alias donorDetectorCounterDefualt: donorDetectorCounterComboBox.defaultStr
    property alias acceptorDetectorCounterDefault: acceptorDetectorCounterComboBox.defaultStr
    property alias donorDetectorPinDefault: donorDetectorPinComboBox.defaultStr
    property alias acceptorDetectorPinDefault: acceptorDetectorPinComboBox.defaultStr
    property alias donorDetectorGateDefault: donorDetectorGateComboBox.defaultStr
    property alias acceptorDetectorGateDefault: acceptorDetectorGateComboBox.defaultStr
    property alias timebaseDefault: timebaseComboBox.defaultStr
    property alias laserControlResolution: laserControlResSpinBox.value
    property alias timestampAdjustmentValue: timestampAdjustmentSpinBox.value

    GridLayout {
        width: Math.max(implicitWidth, scrollView.availableWidth - sbWidth - 10)
        id: contentGrid
        rows: 8
        columns: 2

        columnSpacing: 10

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Device:"
        }

        RowLayout {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.fillHeight: true
            Layout.fillWidth: true

            ComboBox {
                id: deviceComboBox
                Material.theme: Material.Light

                property string defaultStr: "Dev1"

                Layout.fillHeight: true
                Layout.fillWidth: true
                model: dataSource.availableDevices
                onCurrentTextChanged: {
                    if (!currentText === "")
                        defaultStr = currentText
                    dataSource.setCurrentDevice(currentText)
                }
            }

            Button {
                Layout.fillHeight: true

                text: "REFRESH"
                width: 30
                onClicked: dataSource.updateAvailableDevices()
            }
        }



        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Donor Laser Pin:"
        }

        ComboBox {
            id: donorLaserPinComboBox
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            property bool validDevice: false
            property string defaultStr: "port0/line0"

            currentIndex: findIndexOfDefault(model, defaultStr)

            onModelChanged: validDevice = deviceComboBox.currentText !== ""

            model: dataSource.digitalOutLines
            onCurrentTextChanged: {
                if (validDevice)
                    defaultStr = currentText
                dataSource.setDonorLaserPin(currentText)
            }
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Acceptor Laser Pin:"
        }

        ComboBox {
            id: acceptorLaserPinComboBox
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            property bool validDevice: false
            property string defaultStr: "port0/line1"

            currentIndex: findIndexOfDefault(model, defaultStr)

            model: dataSource.digitalOutLines
            onModelChanged: validDevice = deviceComboBox.currentText !== ""

            onCurrentTextChanged: {
                if (validDevice)
                    defaultStr = currentText
                dataSource.setAcceptorLaserPin(currentText)
            }
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Donor Detector Counter:"
        }

        ComboBox {
            id: donorDetectorCounterComboBox
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            property bool validDevice: false
            property string defaultStr: "ctr0"

            currentIndex: findIndexOfDefault(model, defaultStr)

            model: dataSource.counters

            onModelChanged: validDevice = deviceComboBox.currentText !== ""

            onCurrentTextChanged: {
                if (validDevice)
                    defaultStr = currentText
                dataSource.setDonorDetectorCounter(currentText)
            }
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Acceptor Detector Counter:"
        }

        ComboBox {
            id: acceptorDetectorCounterComboBox
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            property bool validDevice: false
            property string defaultStr: "ctr1"

            currentIndex: findIndexOfDefault(model, defaultStr)

            model: dataSource.counters

            onModelChanged: validDevice = deviceComboBox.currentText !== ""

            onCurrentTextChanged: {
                if (validDevice)
                    defaultStr = currentText
                dataSource.setAcceptorDetectorCounter(currentText)
            }
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Donor Detector Pin:"
        }

        ComboBox {
            id: donorDetectorPinComboBox
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            property bool validDevice: false
            property string defaultStr: "PFI1"

            currentIndex: findIndexOfDefault(model, defaultStr)

            model: dataSource.counterLines

            onModelChanged: validDevice = deviceComboBox.currentText !== ""

            onCurrentTextChanged: {
                if (validDevice)
                    defaultStr = currentText
                dataSource.setDonorDetectorPin(currentText)
            }
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Acceptor Detector Pin:"
        }

        ComboBox {
            id: acceptorDetectorPinComboBox
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            property bool validDevice: false
            property string defaultStr: "PFI2"

            currentIndex: findIndexOfDefault(model, defaultStr)

            model: dataSource.counterLines

            onModelChanged: validDevice = deviceComboBox.currentText !== ""

            onCurrentTextChanged: {
                if (validDevice)
                    defaultStr = currentText
                dataSource.setAcceptorDetectorPin(currentText)
            }
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Donor Detector Gate:"
        }

        ComboBox {
            id: donorDetectorGateComboBox
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            property bool validDevice: false
            property string defaultStr: "port0/line8"

            currentIndex: findIndexOfDefault(model, defaultStr)

            model: dataSource.digitalOutLines

            onModelChanged: validDevice = deviceComboBox.currentText !== ""

            onCurrentTextChanged: {
                if (validDevice)
                    defaultStr = currentText
                dataSource.setDonorDetectorGate(currentText)
            }
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Acceptor Detector Gate:"
        }

        ComboBox {
            id: acceptorDetectorGateComboBox
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            property bool validDevice: false
            property string defaultStr: "port0/line9"

            currentIndex: findIndexOfDefault(model, defaultStr)

            model: dataSource.digitalOutLines

            onModelChanged: validDevice = deviceComboBox.currentText !== ""

            onCurrentTextChanged: {
                if(validDevice)
                    defaultStr = currentText
                dataSource.setAcceptorDetectorGate(currentText)
            }
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Timebase:"
        }

        ComboBox {
            id: timebaseComboBox
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            property bool validDevice: false
            property string defaultStr: "100MHzTimebase"

            currentIndex: findIndexOfDefault(model, defaultStr)

            model: dataSource.timebases

            onModelChanged: validDevice = deviceComboBox.currentText !== ""

            onCurrentTextChanged: {
                if (validDevice)
                    defaultStr = currentText
                dataSource.setTimebase(currentText)
            }
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            text: "Laser Control Resolution (ns):"
        }

        SpinBox {
            id: laserControlResSpinBox
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true

            editable: true

            from: 1
            to: 1000000
            value: 1000

            onValueChanged: dataSource.setLaserControlResolution(value)
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            text: "Photon Timestamp Adjustment"
        }

        SpinBox {
            id: timestampAdjustmentSpinBox
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true

            editable: true
            from: 0
            to: 99
            value: 3

            onValueChanged: dataSource.setTimestampAdjustment(value)
        }
    }

    function findIndexOfDefault(model, dflt) {
        for (var indx = 0; indx < model.length; indx++)
            if (model[indx] === dflt) return indx

        return 0
    }

    function sendValuesToNICard() {
        dataSource.setDonorLaserPin(donorLaserPinComboBox.currentText)
        dataSource.setAcceptorLaserPin(acceptorLaserPinComboBox.currentText)
        dataSource.setDonorDetectorCounter(donorDetectorCounterComboBox.currentText)
        dataSource.setAcceptorDetectorCounter(acceptorDetectorCounterComboBox.currentText)
        dataSource.setDonorDetectorPin(donorDetectorPinComboBox.currentText)
        dataSource.setAcceptorDetectorPin(acceptorDetectorPinComboBox.currentText)
        dataSource.setDonorDetectorGate(donorDetectorGateComboBox.currentText)
        dataSource.setAcceptorDetectorGate(acceptorDetectorGateComboBox.currentText)
        dataSource.setTimebase(timebaseComboBox.currentText)
        dataSource.setLaserControlResolution(laserControlResSpinBox.value)
        dataSource.setTimestampAdjustment(timestampAdjustmentSpinBox.value)
    }

    Connections {
        target: dataSource
        onSendValues: sendValuesToNICard()
    }
}
