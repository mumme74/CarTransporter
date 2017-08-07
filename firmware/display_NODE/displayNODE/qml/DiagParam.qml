import QtQuick 2.7
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

    StackLayout {
        id: nodeView
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 10
        height: parent.height - y
        anchors.bottomMargin: 0
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
                width: 450
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
                model: testPIDsModel
            }
        }
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
                width: 450
                TableViewColumn {
                    role: "param"
                    title: qsTr("Param") + tr.str
                    width: 200
                }
                TableViewColumn {
                    role: "value"
                    title: qsTr("Value")+ tr.str
                    width: 100
                }
                model: testPIDsModel
            }
        }
    }


    ListModel {
        id: testPIDsModel
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
