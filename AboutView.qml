import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Item {
    id: aboutView

    ColumnLayout {
        anchors.fill: parent
        implicitHeight: parent.height
        implicitWidth: parent.width

        spacing: aboutView.height / 10


        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: false

            Image {
                Layout.preferredHeight: aboutView.height / 4
                Layout.preferredWidth: aboutView.width / 3


                fillMode: Image.PreserveAspectFit
                source: "UoS.png"
            }

            Image {
                Layout.preferredHeight: aboutView.height / 4
                Layout.preferredWidth: aboutView.width / 3

                fillMode: Image.PreserveAspectFit
                source: "EPSRC.png"
            }

            Image {
                Layout.preferredHeight: aboutView.height / 4
                Layout.preferredWidth: aboutView.width / 3

                fillMode: Image.PreserveAspectFit
                source: "bbsrc.png"
            }
        }


        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            Text {
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true

                fontSizeMode: Text.VerticalFit
                minimumPointSize: 4
                font.pointSize: 100
                wrapMode: Text.Wrap
                color: "white"
                textFormat: Text.StyledText
                onLinkActivated: Qt.openUrlExternally(link)
                linkColor: "lightblue"

                text:  "Version 0.0
                        <br><br>
                        The smfBox was developed in the Craggs Lab, Department of Chemistry, University of Sheffield
                        <br><br>
                        Software developed by Elliot Steele, Cadby Lab, Department of Physics, University of Sheffield
                        <br><br>
                        Operating instructions are available <a href=\"https://craggslab.github.io/smfBox/acqsoftware.html\">here</a>
                        <br><br>
                        Please help continued development by citing our <a href=\"\">paper</a>
                        <br><br>
                        Report all issues on our github <a href=\"https://github.com/craggslab/smfBox\">issue tracker</a>
                        "

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton
                    cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                }
            }

        }
    }
}
