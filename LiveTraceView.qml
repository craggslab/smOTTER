import QtQuick 2.0
import QtCharts 2.3

ChartView {
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark

    property bool acquisitionRunning: false

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
        color: "green"
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

    Timer {
        id: refreshTimer
        interval: 40
        running: acquisitionRunning
        repeat: true

        property int count: 0

        onTriggered: {
            dataSource.updateLiveTrace(countsDD, countsAA, countsDA, axisX.min, count*interval)

            if (count*interval > axisX.max)
            {
                axisX.min = axisX.max
                axisX.max += 1000;
            }
            count++;
        }

        onRunningChanged: {
            count = 0
            if (running)
            {
                countsAA.clear()
                countsDD.clear()
                countsDA.clear()

                axisX.min = 0
                axisX.max = 1000;
            }
        }
    }
}
