import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import "helpers.js" as Helpers

Item {
    id: root

    onVisibleChanged: {
        Helpers.switchToDiagHeader(root);
    }
    PageIndicator {
        id: pageIndicator
        anchors.horizontalCenter: parent.horizontalCenter
        y: 8
        count: nodeView.count
        currentIndex: nodeView.currentIndex
        interactive: true
    }

    IconButton {
        id: setOn
        btnText: qsTr("On")+ tr.str
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 65
    }

    IconButton {
        id: setOff
        btnText: qsTr("Off") + tr.str
        anchors.right: parent.right
        anchors.top: setOn.bottom
        anchors.topMargin: 20
    }

    StackLayout {
        id: nodeView
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: setOff.left
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 10
        height: parent.height - y
        anchors.bottomMargin: 0
        currentIndex: pageIndicator.currentIndex

        Item {
            id: suspensionActivatePage
            Label {
                id: suspensionActivateLbl
                text: qsTr("Suspension activations")+ tr.str
                font.pointSize: 20
                color: "white"
                anchors.top: parent.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            TableView {
                id: suspensionActivateList
                anchors.top: suspensionActivateLbl.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                width: 450
                TableViewColumn {
                    role: "param"
                    title: qsTr("Param")+ tr.str
                    width: 200
                }
                TableViewColumn {
                    role: "value"
                    title: qsTr("Value")+ tr.str
                    width: 100
                }
                model: testActivationModel
            }
        }
        Item {
            id: parkbrakeActivatePage
            Label {
                id: parkbarkeActivationLbl
                text: qsTr("Parkbrake activation")+ tr.str
                font.pointSize: 20
                color: "white"
                anchors.top: parent.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            Label {
                id: parkbrakeHelpLbl
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parkbarkeActivationLbl.bottom
                anchors.topMargin: 5
                color: "beige"
                text: qsTr("You must press brake til brakelights and hold release button") + tr.str
            }

            Grid {
                anchors.top: parkbrakeHelpLbl.bottom
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10
                columns: 2
                Row {
                    IconButton {
                        btnText: qsTr("LF release") + tr.str
                        onClicked: parkbrakeNode.activateOutput(0, false);
                    }
                    IconButton {
                        btnText: qsTr("LF tighten") + tr.str
                        onClicked: parkbrakeNode.activateOutput(0, true);
                    }
                }
                Row {
                    IconButton {
                        btnText: qsTr("RF release") + tr.str
                        onClicked: parkbrakeNode.activateOutput(1, false);
                    }
                    IconButton {
                        btnText: qsTr("RF tighten") + tr.str
                        onClicked: parkbrakeNode.activateOutput(1, true);
                    }
                }
                Row {
                    IconButton {
                        btnText: qsTr("LR release") + tr.str
                        onClicked: parkbrakeNode.activateOutput(2, false);
                    }
                    IconButton {
                        btnText: qsTr("LR tighten") + tr.str
                        onClicked: parkbrakeNode.activateOutput(2, true);
                    }
                }
                Row {
                    IconButton {
                        btnText: qsTr("RR release") + tr.str
                        onClicked: parkbrakeNode.activateOutput(3, false);
                    }
                    IconButton {
                        btnText: qsTr("RR tighten") + tr.str
                        onClicked: parkbrakeNode.activateOutput(3, true);
                    }
                }
            }

            Row {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                IconButton {
                    id: parkbrakeSetServiceBtn
                    btnText: qsTr("Set service mode") + tr.str
                    enabled: !parkbrakeNode.inServiceMode
                    onClicked: parkbrakeNode.setServiceState(true);
                }
                IconButton {
                    id: parkbrakeUnsetServiceBtn
                    btnText: qsTr("Unset service mode") + tr.str
                    enabled: parkbrakeNode.inServiceMode
                    onClicked: parkbrakeNode.setServiceState(false);
                }
            }
        }
    }


    ListModel {
        id: testActivationModel
        ListElement {
            param: "LF current"
            value: "20A"
        }
        ListElement {
            param: "LF Height"
            value: "100steps"
        }
        ListElement {
            param: "LSuck"
            value: "On"
        }
        ListElement {
            param: "Compressor"
            value: "On"
        }
        ListElement {
            param: "Compresor temp"
            value: "100C"
        }
        ListElement {
            param: "state"
            value: "Normal"
        }
    }
}
