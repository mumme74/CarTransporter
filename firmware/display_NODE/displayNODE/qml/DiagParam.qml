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
        anchors.bottom: root.bottom
        anchors.bottomMargin: 0
        anchors.top: root.top
        anchors.topMargin: 10
        anchors.right: root.right
        anchors.rightMargin: 10
        height: root.height - y
        currentIndex: pageIndicator.currentIndex

        Item {
            id: suspensionPIDsPage
            Label {
                id: suspensionPIDsLbl
                text: qsTr("Suspension PIDs")+ tr.str
                font.pointSize: 20
                color: "white"
                anchors.top: suspensionPIDsPage.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            TableView {
                id: suspensionPIDsList
                anchors.top: suspensionPIDsLbl.bottom
                anchors.topMargin: 10
                anchors.bottom: suspensionPIDsPage.bottom
                width: 200
                model: suspensionAvailablePids
                TableViewColumn {
                    role: "param"
                    title: qsTr("Param") + tr.str
                    width: 200
                }
            }
            DiagParamSelected {
                id: suspensionSelected
                node: suspensionNode
                pidsListSelectFrom: suspensionPIDsList
                anchors.top: suspensionPIDsPage.top
                anchors.left: suspensionPIDsList.right
                anchors.right: suspensionPIDsPage.right
                anchors.bottom: suspensionPIDsPage.bottom
            }

        } // end suspension
        Item {
            id: parkbrakePIDsPage
            Label {
                id: parkbarkePIDsLbl
                text: qsTr("Parkbrake PIDs")
                font.pointSize: 20
                color: "white"
                anchors.top: parkbrakePIDsPage.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            TableView {
                id: parkbarkePIDsList
                anchors.top: parkbarkePIDsLbl.bottom
                anchors.topMargin: 10
                anchors.bottom: parkbrakePIDsPage.bottom
                width: 200
                model: parkbrakeAvailablePids
                TableViewColumn {
                    role: "name"
                    title: qsTr("Name") + tr.str
                    width: 200
                }
            }
            DiagParamSelected {
                id: parkbrakeSelected
                node: parkbrakeNode
                pidsListSelectFrom: parkbarkePIDsList
                anchors.top: parkbrakePIDsPage.top
                anchors.left: parkbarkePIDsList.right
                anchors.right: parkbrakePIDsPage.right
                anchors.bottom: parkbrakePIDsPage.bottom
            }
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
