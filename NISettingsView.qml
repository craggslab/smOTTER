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
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: dataSource.digitalOutLines
            onModelChanged: currentIndex = findIndexOfDefault(model, qsTr("port0/line0"))
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Acceptor Laser Pin:"
        }

        ComboBox {
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: dataSource.digitalOutLines
            onModelChanged: currentIndex = findIndexOfDefault(model, qsTr("port0/line1"))
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Donor Detector Counter:"
        }

        ComboBox {
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: dataSource.counters
            onModelChanged: currentIndex = findIndexOfDefault(model, "ctr0")
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Acceptor Detector Counter:"
        }

        ComboBox {
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: dataSource.counters
            onModelChanged: currentIndex = findIndexOfDefault(model, "ctr1")
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Donor Detector Pin:"
        }

        ComboBox {
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: dataSource.counterLines
            onModelChanged: currentIndex = findIndexOfDefault(model, "PFI1");
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Acceptor Detector Pin:"
        }

        ComboBox {
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: dataSource.counterLines
            onModelChanged: currentIndex = findIndexOfDefault(model, "PFI2");
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Donor Detector Gate:"
        }

        ComboBox {
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: dataSource.digitalOutLines
            onModelChanged: currentIndex = findIndexOfDefault(model, "port0/line8")
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Acceptor Detector Gate:"
        }

        ComboBox {
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: dataSource.digitalOutLines
            onModelChanged: currentIndex = findIndexOfDefault(model, "port0/line9")
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            text: "Timebase:"
        }

        ComboBox {
            Material.theme: Material.Light

            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: dataSource.timebases
            onModelChanged: currentIndex = findIndexOfDefault(model, "100MHzTimebase")
        }

        Label {
            Layout.preferredWidth: implicitWidth
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            text: "Laser Control Resolution (us):"
        }

        TextField {
            Layout.alignment: Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            text: qsTr("1")
        }
    }

    function findIndexOfDefault(model, dflt) {
        for (var indx = 0; indx < model.length; indx++)
            if (model[indx] === dflt) return indx

        return 0
    }
}
