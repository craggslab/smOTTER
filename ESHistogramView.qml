import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import HexPlot 1.0

ColumnLayout {

    property bool acquisitionRunning: false
    property alias nBins: binsSlider.value

    HexPlot {
        id: plt
        Layout.fillWidth: true
        Layout.fillHeight: true

        plotLayout.axes.x_label: "FRET Efficiency"
        plotLayout.axes.y_label: "Stoichiometry"
        nBinsX: binsSlider.value
        nBinsY: binsSlider.value

        initialScaleMax: 5
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: false

        Label { text: "DD + DA Threshold: " }
        TextField {
            id: threshold_DD_DA
            horizontalAlignment: TextInput.AlignHCenter

            text: "10"

            validator: IntValidator {
                bottom: 1
                top: 100
            }

            property int value: 10

            onTextChanged: { if (acceptableInput) value = text }

            enabled: !acquisitionRunning
        }
        Label {
            Layout.fillWidth: true
            text: "AA Threshold: "
            horizontalAlignment: Qt.AlignRight
        }
        TextField {
            id: threshold_AA
            horizontalAlignment: TextInput.AlignHCenter

            text: "10"

            validator: IntValidator {
                bottom: 1
                top: 100
            }

            property int value : 10

            onTextChanged: { if (acceptableInput) value = text }

            enabled: !acquisitionRunning
        }
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: false

        Label { text: "Number of Bins: " }
        Slider {
            id: binsSlider
            Layout.fillWidth: true

            from: 10
            to: 100
            value: 30
        }
    }

    property int last_t: 0

    function updateDisplay() {
        var t = dataSource.timeSinceAcqStart()

        dataSource.updateESHistogram(plt, threshold_AA.value, threshold_DD_DA.value, last_t, t)
        last_t = t + 1
    }

    function resetDisplay() {
        plt.clearData()
        last_t = 0
    }
}
