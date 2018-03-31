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
        anchors.horizontalCenter: root.horizontalCenter
        y: 8
        count: nodeView.count
        currentIndex: nodeView.currentIndex
        interactive: true
    }

    StackLayout {
        id: nodeView
        anchors.left: root.left
        anchors.leftMargin: 10
        anchors.right: root.right
        anchors.rightMargin: 10
        anchors.bottom: root.bottom
        anchors.top: root.top
        anchors.topMargin: 10
        height: root.height - y
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
                width: 400
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
                TableViewColumn {
                    role: "unit"
                    title: qsTr("Unit")+ tr.str
                    width: 50
                }
                TableViewColumn {
                    role: "state"
                    title: qsTr("State")+ tr.str
                    width: 50
                }
                model: suspensionActivationModel
                onClicked: {
                    var itm = suspensionActivationModel.get(currentRow)
                    suspensionActivateValueSlider.value = itm ? itm.value : 0
                }
            }
            Slider {
                id: suspensionActivateValueSlider
                anchors.top: suspensionActivateLbl.bottom
                anchors.topMargin: 10
                anchors.left: suspensionActivateList.right
                anchors.leftMargin: 30
                anchors.bottom: parent.bottom
                orientation: Qt.Vertical
                minimumValue: 0
                maximumValue: 100
                stepSize: 1
                onValueChanged: {
                    if (suspensionActivateList.currentRow > -1) {
                        var itm = suspensionActivationModel.get(suspensionActivateList.currentRow)
                        if (itm && itm.value !== undefined)
                            itm.value = value
                        /*suspensionActivationModel.setProperty(
                                suspensionActivateList.currentRow,
                                "value", value)*/
                    }
                }
            }

            IconButton {
                id: setOn
                btnText: qsTr("On")+ tr.str
                anchors.left: suspensionActivateValueSlider.right
                anchors.top: suspensionActivatePage.top
                anchors.topMargin: 65
                anchors.leftMargin: 20
                onClicked: {
                    var itm = suspensionActivationModel.get(suspensionActivateList.currentRow)
                    if (itm && itm.value !== undefined)
                        suspensionNode.activateOutput(itm.pid, itm.value)
                }
            }

            IconButton {
                id: setOff
                btnText: qsTr("Off") + tr.str
                anchors.left: suspensionActivateValueSlider.right
                anchors.leftMargin: 20
                anchors.top: setOn.bottom
                anchors.topMargin: 20
                onClicked: {
                    var itm = suspensionActivationModel.get(suspensionActivateList.currentRow)
                    if (itm)
                        suspensionNode.clearActivateOutput(itm.pid)
                }
            }

            Connections {
                // handles response from our node
                target: suspensionNode
                onActivateOutputConfirmed: {
                    for(var i = 0; i < suspensionActivationModel.count; ++i) {
                        var itm = suspensionActivationModel.get(i);
                        if (itm && itm.pid === pid) {
                            itm.state = "On"
                            itm.value = value
                        }
                    }
                }
                onClearActivateOutputConfirmed: {
                    for(var i = 0; i < suspensionActivationModel.count; ++i) {
                        var itm = suspensionActivationModel.get(i);
                        if (itm && itm.pid === pid) {
                            itm.state = "Off"
                        }
                    }
                }
            }
        } // end suspension
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
                Column {
                    Label {
                        color: "white"
                        text: parkbrakeNode.getPid("LeftFront_state").valueStr
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

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
                }
                Column {
                    Label {
                        color: "white"
                        text: parkbrakeNode.getPid("RightFront_state").valueStr
                        anchors.horizontalCenter: parent.horizontalCenter
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
                }
                Column {
                    Label {
                        color: "white"
                        text: parkbrakeNode.getPid("LeftRear_state").valueStr
                        anchors.horizontalCenter: parent.horizontalCenter
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
                }
                Column {
                    Label {
                        color: "white"
                        text: parkbrakeNode.getPid("RightRear_state").valueStr
                        anchors.horizontalCenter: parent.horizontalCenter
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
            }

            Row {
                id: parkbrakeServiceGrid
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50

                Label {
                    color: "white"
                    text: qsTr("Service mode: %1").arg(parkbrakeNode.serviceMode) + tr.str
                }

                IconButton {
                    id: parkbrakeSetServiceBtn
                    btnText: qsTr("Set service mode") + tr.str
                    enabled: !parkbrakeNode.serviceMode
                    onClicked: parkbrakeNode.setServiceState(true);
                }
                IconButton {
                    id: parkbrakeUnsetServiceBtn
                    btnText: qsTr("Unset service mode") + tr.str
                    enabled: parkbrakeNode.serviceMode
                    onClicked: parkbrakeNode.setServiceState(false);
                }
            }
        }
    }


    ListModel {
        id: suspensionActivationModel
        ListElement {
            param: "LeftFill"
            value: 0
            unit: "%"
            state: "off"
            pid: 1 // From PIDs::IDs ins suspensionNode code
        }
        ListElement {
            param: "LeftDump"
            value: 0
            unit: "%"
            state: "off"
            pid: 2
        }
        ListElement {
            param: "LeftSuck"
            value: 0
            unit: "%"
            state: "off"
            pid: 3
        }
        ListElement {
            param: "RightFill"
            value: 0
            unit: "%"
            state: "off"
            pid: 4
        }
        ListElement {
            param: "RightDump"
            value: 0
            unit: "%"
            state: "off"
            pid: 5
        }
        ListElement {
            param: "RightSuck"
            value: 0
            unit: "%"
            state: "off"
            pid: 6
        }
        ListElement {
            param: "Compressor"
            value: 0
            unit: "%"
            state: "off"
            pid: 7
        }
        ListElement {
            param: "Airdryer"
            value: 0
            unit: "%"
            state: "off"
            pid: 8
        }
        ListElement {
            param: "Spare1"
            value: 0
            unit: "%"
            state: "off"
            pid: 9
        }
    }
}
