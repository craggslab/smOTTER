import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtCharts 2.3

Item {
    property bool acquisitionRunning: false

    RowLayout {

        anchors.fill: parent

        Slider {
            id: yAxisRangeSlider
            Layout.fillWidth: false
            Layout.fillHeight: true

            orientation: Qt.Vertical

            from: 50
            to: 500
            value: 50
            stepSize: 25
        }

        ChartView {
            id: liveChartView
            animationOptions: ChartView.NoAnimation
            theme: ChartView.ChartThemeDark

            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: dataSource.removeSeriesFromLegend(legend, currentPositionMarker)

            ValueAxis {
                id: axisY
                min: -Math.floor(yAxisRangeSlider.value)
                max: Math.floor(yAxisRangeSlider.value)
                titleText: "Counts"
            }

            ValueAxis {
                id: axisX
                min: 0
                max: 1000
                titleText: "Time (ms)"
            }

            LineSeries {
                id: countsDD
                name: "DD"
                axisX: axisX
                axisY: axisY
                color: "lime"
                useOpenGL: true
            }

            LineSeries {
                id: countsAA
                name: "AA"
                axisX: axisX
                axisY: axisY
                color: "yellow"
                useOpenGL: true
            }

            LineSeries{
                id: countsDA
                name: "DA"
                axisX: axisX
                axisY: axisY
                color: "red"
                useOpenGL: true
            }

            LineSeries {
                id: currentPositionMarker
                axisX: axisX
                axisY: axisY
                color: "grey"
                useOpenGL: true
            }
        }
    }

    function updateDisplay() {
        var t = Math.max(0, dataSource.timeSinceAcqStart() - 500);
        dataSource.updateLiveTrace(countsDD, countsAA, countsDA, currentPositionMarker, axisX.min, t, 1000, axisY.min, axisY.max)

        if (t > axisX.max)
        {
            axisX.min = axisX.max
            axisX.max += 1000;
        }
    }

    function resetDisplay() {
        countsAA.clear()
        countsDD.clear()
        countsDA.clear()

        axisX.min = 0
        axisX.max = 1000
    }

}
