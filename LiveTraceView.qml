import QtQuick 2.0
import QtCharts 2.3

ChartView {
    id: liveChartView
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark

    property bool acquisitionRunning: false

    Component.onCompleted: dataSource.removeSeriesFromLegend(legend, currentPositionMarker)

    ValueAxis {
        id: axisY
        min: -50
        max: 50
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
