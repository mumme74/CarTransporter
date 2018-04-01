import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import "helpers.js" as Helpers
import mummesoft 0.1

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

    StackLayout {
        id: nodeView
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 10
        height: parent.height - y
        anchors.bottomMargin: 0
        width: 200
        currentIndex: pageIndicator.currentIndex

        Item {
            id: suspensionPIDsPage
            Label {
                id: suspensionPIDsLbl
                text: qsTr("Suspension PIDs")+ tr.str
                font.pointSize: 20
                color: "white"
                anchors.top: parent.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            TableView {
                id: suspensionPIDsList
                anchors.top: suspensionPIDsLbl.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                width: nodeView.width
                model: suspensionAvailablePids
                TableViewColumn {
                    role: "param"
                    title: qsTr("Param") + tr.str
                    width: 200
                }
                TableViewColumn {
                    role: "value"
                    title: qsTr("Value") + tr.str
                    width: 100
                }
            }
        } // end suspension
        Item {
            id: parkbrakePIDsPage
            Label {
                id: parkbarkePIDsLbl
                text: qsTr("Parkbrake PIDs")
                font.pointSize: 20
                color: "white"
                anchors.top: parent.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            TableView {
                id: parkbarkePIDsList
                anchors.top: parkbarkePIDsLbl.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                width: nodeView.width
                model: parkbrakeAvailablePids
                TableViewColumn {
                    role: "name"
                    title: qsTr("Name") + tr.str
                    width: 200
                }
            }
        }
    }

    IconButton {
        id: selectBtn
        anchors.left: nodeView.right
        anchors.leftMargin: 5
        anchors.verticalCenter: nodeView.verticalCenter
        anchors.verticalCenterOffset: -30
        btnText: ">>"
        onClicked: {
            var pid = null;
            if (nodeView.currentIndex == 0) {
                pid = suspensionNode.getPid(suspensionPIDsList.currentRow);
            } else if (nodeView.currentIndex == 1) {
                pid = parkbrakeNode.getPid(parkbarkePIDsList.currentRow);

            }
            console.log(pid, pid.key);

            if (pid)
                canPidsModel.addPid(pid);
        }
    }

    IconButton {
        id: deSelectBtn
        anchors.left: nodeView.right
        anchors.leftMargin: 5
        anchors.verticalCenter: nodeView.verticalCenter
        anchors.verticalCenterOffset: 30
        btnText: ">!<"
        onClicked: {
            var pid = canPidsModel.getPid(selectedPIDsList.currentRow);
            canPidsModel.removePid(pid);
            console.log(pid, pid.key)
        }
    }

    Label {
       id: selectedPIDsLbl
       text: qsTr("Selected PIDs")
       font.pointSize: 20
       color: "white"
       anchors.top: parent.top
       anchors.topMargin: pageIndicator.y + pageIndicator.height
       anchors.right: parent.right
       anchors.rightMargin: 10
    }
    TableView {
        id: selectedPIDsList
        anchors.top: selectedPIDsLbl.bottom
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: deSelectBtn.right
        anchors.leftMargin: 10
        model: canPidsModel
        TableViewColumn {
            role: "name"
            title: qsTr("Name") + tr.str
            width: 200
        }
        TableViewColumn {
            role: "value"
            title: qsTr("Value") + tr.str
            width: 100
        }
        TableViewColumn {
            role: "origin"
            title: qsTr("Origin") + tr.str
            width: 200
        }
    }


    // currently avaliable PIDs in parkbrake
    ListModel {
        id: parkbrakeAvailablePids
        Component.onCompleted: {
            var pidsList = parkbrakeNode.pids;
            for (var i = 0; i < pidsList.length; ++i) {
                parkbrakeAvailablePids.append({name: pidsList[i].key });
            }
        }

    }
    // add new PIDs as they appear
    Connections {
        target: parkbrakeNode
        onNewPid: {
            parkbrakeAvailablePids.append({name: pid.key});
        }
    }

    // currently avaliable PIDs in parkbrake
    ListModel {
        id: suspensionAvailablePids

        Component.onCompleted: {
            var pidsList = suspensionNode.pids;
            for (var i = 0; i < pidsList.length; ++i) {
                suspensionAvailablePids.append({name: pidsList[i].key });
            }
        }
    }
    // add new PIDs as they appear
    Connections {
        target: suspensionNode
        onNewPid: {
            suspensionAvailablePids.append({name: pid.key});
        }
    }
}
