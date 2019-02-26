import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.3

ScrollView {
    id: scrollView

    property alias donorLabel: donorLabelTextField.text
    property alias acceptorLabel: acceptorLabelTextField.text
    property alias donorExWavelength: donorExWavelengthTextField.text
    property alias acceptorExWavelength: acceptorExWavelengthTextField.text
    property alias donorDetWavelength: donorDetWavelengthTextField.text
    property alias acceptorDetWavelength: acceptorDetWavelengthTextField.text
    property alias donorLaserPower: donorLaserPowerTextField.text
    property alias acceptorLaserPower: acceptorLaserPowerTextField.text
    property alias userName: userTextField.text
    property alias userAffiliation: userAffiliationTextField.text

    property int sbWidth: ScrollBar.vertical.width
    ScrollBar.vertical.policy: ScrollBar.AlwaysOn

    clip: true

    GridLayout {
        width: Math.max(implicitWidth, scrollView.availableWidth - sbWidth - 10)
        id: contentGrid
        columns: 2

        columnSpacing: 10

        RowLayout {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.columnSpan: 2

            TextField {
                id: fileTextField

                Layout.fillHeight: true
                Layout.fillWidth: true

                placeholderText: "Save File Name"
                onTextChanged: {
                    dataSource.setFilename(text)
                }

                // Work around for display bug
                background: Item {
                    implicitWidth: 120
                    Rectangle {
                        y: fileTextField.height - height - fileTextField.bottomPadding + 8
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: fileTextField.activeFocus || fileTextField.hovered ? 2 : 1
                        color: fileTextField.activeFocus ? fileTextField.Material.accentColor
                                               : (fileTextField.hovered ? fileTextField.Material.primaryTextColor : fileTextField.Material.hintTextColor)
                    }
                }
            }

            Button {
                Layout.fillHeight: true

                text: qsTr("...")
                width: 30
                onClicked: folderDialog.open()
            }

            FileDialog {
                id: folderDialog
                title: qsTr("Choose a folder")
                folder: shortcuts.home
                nameFilters: [ "Photon-HDF5 Files (*.h5 *.hdf5)" ]
                selectExisting: false
                sidebarVisible: true
                onAccepted: {
                    fileTextField.text = fileUrl.toString().replace(/^(file:\/{3})/,"");
                }
            }
        }

        Label {
            Layout.columnSpan: 2
            text: qsTr("Sample Name")
        }

        TextField {
            id: sampleNameTextField
            Layout.columnSpan: 2
            Layout.fillWidth: true

            onTextChanged: {
                dataSource.setSampleName(text)
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: sampleNameTextField.height - height - sampleNameTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: sampleNameTextField.activeFocus || sampleNameTextField.hovered ? 2 : 1
                    color: sampleNameTextField.activeFocus ? sampleNameTextField.Material.accentColor
                           : (sampleNameTextField.hovered ? sampleNameTextField.Material.primaryTextColor : sampleNameTextField.Material.hintTextColor)
                }
            }
        }

        Label {
            Layout.columnSpan: 2
            text: qsTr("Sample Details")
        }

        TextArea {
            id: sampleDetailsTextArea
            Layout.columnSpan: 2
            Layout.fillWidth: true

            onTextChanged: {
                dataSource.setDescription(text)
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Donor Label")
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Acceptor Label")
        }


        TextField {
            id: donorLabelTextField
            Layout.fillWidth: true

            onTextChanged: {
                dataSource.setDonorDye(text)
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: donorLabelTextField.height - height - donorLabelTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: donorLabelTextField.activeFocus || donorLabelTextField.hovered ? 2 : 1
                    color: donorLabelTextField.activeFocus ? donorLabelTextField.Material.accentColor
                           : (donorLabelTextField.hovered ? donorLabelTextField.Material.primaryTextColor : donorLabelTextField.Material.hintTextColor)
                }
            }
        }

        TextField {
            id: acceptorLabelTextField
            Layout.fillWidth: true

            onTextChanged: {
                dataSource.setAcceptorDye(text)
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: acceptorLabelTextField.height - height - acceptorLabelTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: acceptorLabelTextField.activeFocus || acceptorLabelTextField.hovered ? 2 : 1
                    color: acceptorLabelTextField.activeFocus ? acceptorLabelTextField.Material.accentColor
                           : (acceptorLabelTextField.hovered ? acceptorLabelTextField.Material.primaryTextColor : acceptorLabelTextField.Material.hintTextColor)
                }
            }
        }

        Label {
            Layout.columnSpan: 2
            text: qsTr("Buffer")
        }

        TextField {
            id: bufferTextField
            Layout.columnSpan: 2
            Layout.fillWidth: true

            onTextChanged: {
                dataSource.setBufferDesc(text)
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: bufferTextField.height - height - bufferTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: bufferTextField.activeFocus || bufferTextField.hovered ? 2 : 1
                    color: bufferTextField.activeFocus ? bufferTextField.Material.accentColor
                           : (bufferTextField.hovered ? bufferTextField.Material.primaryTextColor : bufferTextField.Material.hintTextColor)
                }
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Donor Excitation Wavelength (nm)")
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Acceptor Excitation Wavelength (nm)")
        }

        TextField {
            id: donorExWavelengthTextField
            Layout.fillWidth: true
            validator: DoubleValidator { }

            onTextChanged: {
                dataSource.setDonorExWavelength(text)
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: donorExWavelengthTextField.height - height - donorExWavelengthTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: donorExWavelengthTextField.activeFocus || donorExWavelengthTextField.hovered ? 2 : 1
                    color: donorExWavelengthTextField.activeFocus ? donorExWavelengthTextField.Material.accentColor
                           : (donorExWavelengthTextField.hovered ? donorExWavelengthTextField.Material.primaryTextColor : donorExWavelengthTextField.Material.hintTextColor)
                }
            }
        }

        TextField {
            id: acceptorExWavelengthTextField
            Layout.fillWidth: true
            validator: DoubleValidator {}

            onTextChanged: {
                dataSource.setAcceptorExWavelength(text)
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: acceptorExWavelengthTextField.height - height - acceptorExWavelengthTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: acceptorExWavelengthTextField.activeFocus || acceptorExWavelengthTextField.hovered ? 2 : 1
                    color: acceptorExWavelengthTextField.activeFocus ? acceptorExWavelengthTextField.Material.accentColor
                           : (acceptorExWavelengthTextField.hovered ? acceptorExWavelengthTextField.Material.primaryTextColor : acceptorExWavelengthTextField.Material.hintTextColor)
                }
            }

        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Donor Detection Wavelength (nm)")
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Acceptor Detection Wavelength (nm)")
        }

        TextField {
            id: donorDetWavelengthTextField
            Layout.fillWidth: true
            validator: DoubleValidator { }

            onTextChanged: {
                dataSource.setDonorDetWavelength(text)
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: donorDetWavelengthTextField.height - height - donorDetWavelengthTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: donorDetWavelengthTextField.activeFocus || donorDetWavelengthTextField.hovered ? 2 : 1
                    color: donorDetWavelengthTextField.activeFocus ? donorDetWavelengthTextField.Material.accentColor
                           : (donorDetWavelengthTextField.hovered ? donorDetWavelengthTextField.Material.primaryTextColor : donorDetWavelengthTextField.Material.hintTextColor)
                }
            }
        }

        TextField {
            id: acceptorDetWavelengthTextField
            Layout.fillWidth: true
            validator: DoubleValidator {}

            onTextChanged: {
                dataSource.setAcceptorDetWavelength(text)
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: acceptorDetWavelengthTextField.height - height - acceptorDetWavelengthTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: acceptorDetWavelengthTextField.activeFocus || acceptorDetWavelengthTextField.hovered ? 2 : 1
                    color: acceptorDetWavelengthTextField.activeFocus ? acceptorDetWavelengthTextField.Material.accentColor
                           : (acceptorDetWavelengthTextField.hovered ? acceptorDetWavelengthTextField.Material.primaryTextColor : acceptorDetWavelengthTextField.Material.hintTextColor)
                }
            }

        }
        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Donor Laser Power (mW)")
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Acceptor Laser Power (mW)")
        }

        TextField {
            id: donorLaserPowerTextField
            Layout.fillWidth: true
            validator: DoubleValidator { }

            onTextChanged: {
                dataSource.setDonorInputPower(text);
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: donorLaserPowerTextField.height - height - donorLaserPowerTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: donorLaserPowerTextField.activeFocus || donorLaserPowerTextField.hovered ? 2 : 1
                    color: donorLaserPowerTextField.activeFocus ? donorLaserPowerTextField.Material.accentColor
                           : (donorLaserPowerTextField.hovered ? donorLaserPowerTextField.Material.primaryTextColor : donorLaserPowerTextField.Material.hintTextColor)
                }
            }
        }

        TextField {
            id: acceptorLaserPowerTextField
            Layout.fillWidth: true
            validator: DoubleValidator {}

            onTextChanged: {
                dataSource.setAcceptorInputPower(text);
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: acceptorLaserPowerTextField.height - height - acceptorLaserPowerTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: acceptorLaserPowerTextField.activeFocus || acceptorLaserPowerTextField.hovered ? 2 : 1
                    color: acceptorLaserPowerTextField.activeFocus ? acceptorLaserPowerTextField.Material.accentColor
                           : (acceptorLaserPowerTextField.hovered ? acceptorLaserPowerTextField.Material.primaryTextColor : acceptorLaserPowerTextField.Material.hintTextColor)
                }
            }

        }

        Label {
            Layout.columnSpan: 2
            text: qsTr("User(s)")
        }

        TextField {
            id: userTextField
            Layout.columnSpan: 2
            Layout.fillWidth: true

            onTextChanged: {
                dataSource.setUserName(text);
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: userTextField.height - height - userTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: userTextField.activeFocus || userTextField.hovered ? 2 : 1
                    color: userTextField.activeFocus ? userTextField.Material.accentColor
                           : (userTextField.hovered ? userTextField.Material.primaryTextColor : userTextField.Material.hintTextColor)
                }
            }
        }

        Label {
            Layout.columnSpan: 2
            text: qsTr("User Affiliation")
        }

        TextField {
            id: userAffiliationTextField
            Layout.fillWidth: true
            Layout.columnSpan: 2

            onTextChanged: {
                dataSource.setUserAffiliation(text);
            }

            // Work around for display bug
            background: Item {
                implicitWidth: 120
                Rectangle {
                    y: userAffiliationTextField.height - height - userAffiliationTextField.bottomPadding + 8
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: userAffiliationTextField.activeFocus || userAffiliationTextField.hovered ? 2 : 1
                    color: userAffiliationTextField.activeFocus ? userAffiliationTextField.Material.accentColor
                           : (userAffiliationTextField.hovered ? userAffiliationTextField.Material.primaryTextColor : userAffiliationTextField.Material.hintTextColor)
                }
            }
        }

        Button {
            text: "Test"
            onClicked: dataSource.saveNewPhotons()
        }
    }

}
