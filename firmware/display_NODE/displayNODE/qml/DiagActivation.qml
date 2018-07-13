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

    //StackLayout {
    SwipeView {
        id: nodeView
        spacing: Helpers.mainView().width - width
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
                        if (!suspensionNode.activateOutput(itm.pid, itm.value))
                            Helpers.notifyCanIsOff();
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
                        if (!suspensionNode.clearActivateOutput(itm.pid))
                            Helpers.notifyCanIsOff();
                }
            }

            Connections {
                // handles response from our node
                target: suspensionNode
                onActivateOutputComfirmed: {
                    for(var i = 0; i < suspensionActivationModel.count; ++i) {
                        var itm = suspensionActivationModel.get(i);
                        if (itm && itm.pid === pid) {
                            itm.state = "On"
                            itm.value = value
                        }
                    }
                }
            }
            Connections {
                target: suspensionNode
                onClearActivateOutputComfirmed: {
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
                anchors.top: parkbrakeActivatePage.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            Label {
                id: parkbrakeHelpLbl
                anchors.horizontalCenter: parkbrakeActivatePage.horizontalCenter
                anchors.top: parkbarkeActivationLbl.bottom
                anchors.topMargin: 5
                color: "beige"
                text: qsTr("You must press brake til brakelights and hold release button") + tr.str
            }

            function parkbrakeActivate(output, state) {
                if (!parkbrakeNode.activateOutput(output, state))
                    Helpers.notifyCanIsOff();
            }

            function parkbrakeSetServiceState(state) {
                if (!parkbrakeNode.setServiceState(state))
                    Helpers.notifyCanIsOff();
            }

            Grid {
                anchors.top: parkbrakeHelpLbl.bottom
                anchors.topMargin: 10
                anchors.left: parkbrakeActivatePage.left
                anchors.right: parkbrakeActivatePage.right
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
                            onClicked: parkbrakeActivate(0, false);
                        }
                        IconButton {
                            btnText: qsTr("LF tighten") + tr.str
                            onClicked: parkbrakeActivate(0, true);
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
                            onClicked: parkbrakeActivate(1, false);
                        }
                        IconButton {
                            btnText: qsTr("RF tighten") + tr.str
                            onClicked: parkbrakeActivate(1, true);
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
                            onClicked: parkbrakeActivate(2, false);
                        }
                        IconButton {
                            btnText: qsTr("LR tighten") + tr.str
                            onClicked: parkbrakeActivate(2, true);
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
                            onClicked: parkbrakeActivate(3, false);
                        }
                        IconButton {
                            btnText: qsTr("RR tighten") + tr.str
                            onClicked: parkbrakeActivate(3, true);
                        }
                    }
                }
            }

            Row {
                id: parkbrakeServiceGrid
                anchors.left: parkbrakeActivatePage.left
                anchors.right: parkbrakeActivatePage.right
                anchors.bottom: parkbrakeActivatePage.bottom
                anchors.bottomMargin: 50

                Label {
                    color: "white"
                    text: qsTr("Service mode: %1").arg(parkbrakeNode.serviceMode) + tr.str
                }

                IconButton {
                    id: parkbrakeSetServiceBtn
                    btnText: qsTr("Set service mode") + tr.str
                    enabled: !parkbrakeNode.serviceMode
                    onClicked: parkbrakeSetServiceState(true);
                }
                IconButton {
                    id: parkbrakeUnsetServiceBtn
                    btnText: qsTr("Unset service mode") + tr.str
                    enabled: parkbrakeNode.serviceMode
                    onClicked: parkbrakeSetServiceState(false);
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
            param: "Airdryer"
            value: 0
            unit: "%"
            state: "off"
            pid: 7
        }
        ListElement {
            param: "Spare1"
            value: 0
            unit: "%"
            state: "off"
            pid: 8
        }
        ListElement {
            param: "Compressor"
            value: 0
            unit: "%"
            state: "off"
            pid: 9
        }
    }
}
