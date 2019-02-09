import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

GridLayout {
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
        Layout.alignment: Qt.AlignHCenter
        value: 0
    }

    Label {
        Layout.alignment: Qt.AlignHCenter
        text: qsTr("OFF")
        font.bold: true
    }

    SpinBox {
        Layout.alignment: Qt.AlignHCenter
        value: 50
    }


    SpinBox {
        Layout.alignment: Qt.AlignHCenter
        value: 45
    }

    Label {
        Layout.alignment: Qt.AlignHCenter
        text: qsTr("ON")
        font.bold: true
    }

    SpinBox {
        Layout.alignment: Qt.AlignHCenter
        value: 45
    }


    SpinBox {
        Layout.alignment: Qt.AlignHCenter
        value: 55
    }

    Label {
        Layout.alignment: Qt.AlignHCenter
        text: qsTr("OFF")
        font.bold: true
    }

    SpinBox {
        Layout.alignment: Qt.AlignHCenter
        value: 5
    }

    Label {
        Layout.alignment: Qt.AlignRight
        text: qsTr("ALEX Period (us):")
    }

    SpinBox {
        Layout.alignment: Qt.AlignLeft
        Layout.columnSpan: 2

        from: 1
        to: 500
        value: 100
    }
}
