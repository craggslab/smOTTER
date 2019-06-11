import QtQuick 2.0

Item {
    width: 640
    height: 480

    Loader {
        id: loader
        anchors.fill: parent
        sourceComponent: splashScreen
    }

    Component {
        id: splashScreen

        MouseArea {
            Component.onCompleted: mainWindow.flags = Qt.SplashScreen

            Image {
                id: img
                source: "splash.png"
            }

            onClicked: loader.source = "main.qml"
        }
    }

    Timer {
        interval: 3000
        running: true

        onTriggered: loader.source = "main.qml"
    }

}
