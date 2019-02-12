import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3

ScrollView {
    id: scrollView

    property int sbWidth: ScrollBar.vertical.width

    ScrollBar.vertical.policy: ScrollBar.AlwaysOn

    clip: true

    GridLayout {
        width: Math.max(implicitWidth, scrollView.availableWidth - sbWidth - 10)
        id: contentGrid
        rows: 7
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
                Material.theme: Material.Light

                Layout.fillHeight: true
                Layout.fillWidth: true
                model: dataSource.availableDevices
                onCurrentTextChanged: dataSource.setCurrentDevice(currentText)
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

            model: dataSource.digitalOutLines
            onModelChanged: currentIndex = findIndexOfDefault(model, qsTr("port0/line0"))
            onCurrentTextChanged: dataSource.setDonorLaserPin(currentText)
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

            model: dataSource.digitalOutLines
            onModelChanged: currentIndex = findIndexOfDefault(model, qsTr("port0/line1"))
            onCurrentTextChanged: dataSource.setAcceptorLaserPin(currentText)
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

            model: dataSource.counters
            onModelChanged: currentIndex = findIndexOfDefault(model, "ctr0")
            onCurrentTextChanged: dataSource.setDonorDetectorCounter(currentText)
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

            model: dataSource.counters
            onModelChanged: currentIndex = findIndexOfDefault(model, "ctr1")
            onCurrentTextChanged: dataSource.setAcceptorDetectorCounter(currentText)
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

            model: dataSource.counterLines
            onModelChanged: currentIndex = findIndexOfDefault(model, "PFI1");
            onCurrentTextChanged: dataSource.setDonorDetectorPin(currentText)
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

            model: dataSource.counterLines
            onModelChanged: currentIndex = findIndexOfDefault(model, "PFI2");
            onCurrentTextChanged: dataSource.setAcceptorDetectorPin(currentText)
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

            model: dataSource.digitalOutLines
            onModelChanged: currentIndex = findIndexOfDefault(model, "port0/line8")
            onCurrentTextChanged: dataSource.setDonorDetectorGate(currentText)
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

            model: dataSource.digitalOutLines
            onModelChanged: currentIndex = findIndexOfDefault(model, "port0/line9")
            onCurrentTextChanged: dataSource.setAcceptorDetectorGate(currentText)
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

            model: dataSource.timebases
            onModelChanged: currentIndex = findIndexOfDefault(model, "100MHzTimebase")
            onCurrentTextChanged: dataSource.setTimebase(currentText)
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
    }

    Connections {
        target: dataSource
        onSendValues: sendValuesToNICard()
    }
}
