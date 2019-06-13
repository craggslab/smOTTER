import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtCharts 2.3

Item {
    property bool acquisitionRunning: false

   ColumnLayout {
       anchors.fill: parent


       RowLayout {
           Layout.fillHeight: true
           Layout.fillWidth: true

           Slider {
               id: yAxisRangeSlider
               Layout.fillWidth: false
               Layout.fillHeight: true

               orientation: Qt.Vertical

               from: 10
               to: 200
               value: 50
           }

           ChartView {
               Layout.fillWidth: true
               Layout.fillHeight: true

               animationOptions: ChartView.NoAnimation
               theme: ChartView.ChartThemeDark

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
                   id: countsD
                   name: "Donor Channel"
                   axisX: axisX
                   axisY: axisY
                   color: "lime"
                   useOpenGL: true
               }

               LineSeries {
                   id: countsA
                   name: "Acceptor Channel"
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

       RowLayout {
           Layout.fillWidth: true
           Layout.fillHeight: false

           Layout.alignment: Qt.AlignCenter

           Label {
               text: "Binning (ms): "
           }

           SpinBox {
               id: binningSpinBox
               from: 1
               to: 30
               value: 20
               enabled: !acquisitionRunning
               editable: true
           }

           Label {
               text: "X Range (s): "
           }

           SpinBox {
               id: xRangeSpinBox
               from: 1
               to: 60
               value: 30
               enabled: !acquisitionRunning
               editable: true
           }
       }
   }

   function updateDisplay() {
        var t = dataSource.timeSinceAcqStart()
        dataSource.updateAlignmentTrace(countsD, countsA, currentPositionMarker, binningSpinBox.value, axisX.min, t, xRangeSpinBox.value * 1000, axisY.min, axisY.max)

        if (t > axisX.max)
        {
            axisX.min = axisX.max
            axisX.max += xRangeSpinBox.value * 1000;
        }
    }

    function resetDisplay() {
        countsD.clear()
        countsA.clear()

        axisX.min = 0
        axisX.max = xRangeSpinBox.value * 1000;
    }
}
