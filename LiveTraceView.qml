import QtQuick 2.0
import QtCharts 2.3

ChartView {
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark

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
        color: "red"
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
        color: "green"
        useOpenGL: true
    }

    Timer {
        id: refreshTimer
        interval: 1
        running: true
        repeat: true

        onTriggered: {
            dataSource.updateLiveTrace(countsDD, countsAA, countsDA, 0.0)
        }
    }
}
