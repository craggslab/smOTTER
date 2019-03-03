import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

ScrollView {
    id: scrollView
    property int sbWidth: ScrollBar.vertical.width

    clip:true


    property alias donorOffPercent: donorOffTimeSpinBox.value
    property alias donorOnPercent: donorOnTimeSpinBox.value
    property alias acceptorOffPercent: acceptorOffTimeSpinBox.value
    property alias acceptorOnPercent: acceptorOnTimeSpinBox.value
    property alias alexPeriod: alexPeriodSpinBox.value

    GridLayout {
        width: Math.max(implicitWidth, scrollView.availableWidth - sbWidth - 10)

        rows: 5
        columns: 3

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Donor Duty Cycle:"
        }

        Label {}

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Acceptor Duty Cycle:")
        }

        SpinBox {
            id: donorOffTimeSpinBox
            Layout.alignment: Qt.AlignHCenter
            editable: true

            onValueChanged: dataSource.setDonorLaserOffPercentage(value)

            value: 0
            from: 0
            to: 100 - donorOnTimeSpinBox.value
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("OFF")
            font.bold: true
        }

        SpinBox {
            id: acceptorOffTimeSpinBox
            Layout.alignment: Qt.AlignHCenter
            editable: true

            onValueChanged: dataSource.setAcceptorLaserOffPercentage(value)

            value: 50
            from: 0
            to: 100 - acceptorOnTimeSpinBox.value
        }


        SpinBox {
            id: donorOnTimeSpinBox
            Layout.alignment: Qt.AlignHCenter
            editable: true

            onValueChanged: dataSource.setDonorLaserOnPercentage(value)

            value: 45
            from: 0
            to: 100 - donorOffTimeSpinBox.value
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("ON")
            font.bold: true
        }

        SpinBox {
            id: acceptorOnTimeSpinBox
            Layout.alignment: Qt.AlignHCenter
            editable: true

            onValueChanged: dataSource.setAcceptorLaserOnPercentage(value)

            value: 45
            from: 0
            to: 100 - acceptorOffTimeSpinBox.value
        }


        SpinBox {
            Layout.alignment: Qt.AlignHCenter
            enabled: false
            value: 100 - donorOffTimeSpinBox.value - donorOnTimeSpinBox.value
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("OFF")
            font.bold: true
        }

        SpinBox {
            Layout.alignment: Qt.AlignHCenter
            enabled: false
            value: 100 - acceptorOffTimeSpinBox.value - acceptorOnTimeSpinBox.value
        }

        Label {
            Layout.alignment: Qt.AlignRight
            text: qsTr("ALEX Period (us):")
        }

        SpinBox {
            id: alexPeriodSpinBox
            Layout.alignment: Qt.AlignLeft
            Layout.columnSpan: 2

            onValueChanged: dataSource.setAlexPeriod(value)

            from: 1
            to: 500
            value: 100
            editable: true
        }
    }

    function sendValuesToNICard()
    {
        dataSource.setAlexPeriod(alexPeriodSpinBox.value)
        dataSource.setDonorLaserOffPercentage(donorOffTimeSpinBox.value)
        dataSource.setDonorLaserOnPercentage(donorOnTimeSpinBox.value)
        dataSource.setAcceptorLaserOffPercentage(acceptorOffTimeSpinBox.value)
        dataSource.setAcceptorLaserOnPercentage(donorOnTimeSpinBox.value)
    }


    Connections {
        target: dataSource
        onSendValues: sendValuesToNICard()
    }
}
