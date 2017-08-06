import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
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
        id: readDtcs
        btnText: qsTr("Read")
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 65
    }

    IconButton {
        id: eraseDtcs
        btnText: qsTr("Erase")
        anchors.left: parent.left
        anchors.top: readDtcs.bottom
        anchors.topMargin: 20
    }

    StackLayout {
        id: nodeView
        anchors.left: eraseDtcs.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 10
        height: parent.height - y
        anchors.bottomMargin: 0
        currentIndex: pageIndicator.currentIndex

        Item {
            id: suspensionDtcPage
            Label {
                id: suspensionDtcLbl
                text: qsTr("Suspension DTC")
                font.pointSize: 20
                color: "white"
                anchors.top: parent.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            TableView {
                id: suspensionDtcList
                anchors.top: suspensionDtcLbl.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                width: 450
                TableViewColumn {
                    role: "code"
                    title: "Code"
                    width: 70
                }
                TableViewColumn {
                    role: "desc"
                    title: "Description"
                    width: 300
                }
                model: testDtcModel
            }
            Label {
                id: suspensionFFLbl
                text: qsTr("Freeze Frame")
                color: "white"
                font.pointSize: 20
                anchors.horizontalCenter: suspensionFreezeFrameList.horizontalCenter
                anchors.top: suspensionDtcLbl.top
            }
            TableView {
                id: suspensionFreezeFrameList
                anchors.top: suspensionDtcList.top
                anchors.left: suspensionDtcList.right
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                TableViewColumn {
                    role: "param"
                    title: "Param"
                    width: 100
                }
                TableViewColumn {
                    role: "value"
                    title: "Value"
                    width: 80
                }
                model: testFreezeFrameModel
            }
        }
        Item {
            id: parkbrakeDtcPage
            Label {
                id: parkbarkeDtcLbl
                text: qsTr("Parkbrake DTC")
                font.pointSize: 20
                color: "white"
                anchors.top: parent.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            TableView {
                id: parkbarkeDtcList
                anchors.top: parkbarkeDtcLbl.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                width: 450
                TableViewColumn {
                    role: "code"
                    title: "Code"
                    width: 70
                }
                TableViewColumn {
                    role: "desc"
                    title: "Description"
                    width: 300
                }
                model: testDtcModel
            }
            Label {
                id: parkbarkeFFLbl
                text: qsTr("Freeze Frame")
                color: "white"
                font.pointSize: 20
                anchors.horizontalCenter: parkbarkeFreezeFrameList.horizontalCenter
                anchors.top: parkbarkeDtcLbl.top
            }
            TableView {
                id: parkbarkeFreezeFrameList
                anchors.top: parkbarkeDtcList.top
                anchors.left: parkbarkeDtcList.right
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                TableViewColumn {
                    role: "param"
                    title: "Param"
                    width: 100
                }
                TableViewColumn {
                    role: "value"
                    title: "Value"
                    width: 80
                }
                model: testFreezeFrameModel
            }
        }
    }


    // these can be erased when bridge to C++ is finished
    ListModel {
        id: testDtcModel
        ListElement {
            code: "0x1F0A"
            desc: "Dtc:1"
        }
        ListElement {
            code: "0xFF01"
            desc: "My 2 DTC"
        }
        ListElement {
            code: "0x0234"
            desc: "This is stupid DTC"
        }
    }

    ListModel {
        id: testFreezeFrameModel
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
