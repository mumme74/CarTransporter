import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
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
        btnText: qsTr("Read") + tr.str
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 65
        onClicked: parkbrakeNode.fetchAllDtcs();
    }

    IconButton {
        id: eraseDtcs
        btnText: qsTr("Erase") + tr.str
        anchors.left: parent.left
        anchors.top: readDtcs.bottom
        anchors.topMargin: 20
        onClicked: parkbrakeNode.clearAllDtcs();
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
                text: qsTr("Suspension DTC")+ tr.str
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
                model: testDtcModel
                TableViewColumn {
                    role: "code"
                    title: qsTr("Code")+ tr.str
                    width: 70
                }
                TableViewColumn {
                    role: "desc"
                    title: qsTr("Description")+ tr.str
                    width: 300
                }
            }
            Label {
                id: suspensionFFLbl
                text: qsTr("Freeze Frame")+ tr.str
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
                    title: qsTr("Param")+ tr.str
                    width: 100
                }
                TableViewColumn {
                    role: "value"
                    title: qsTr("Value")+ tr.str
                    width: 80
                }
                model: testFreezeFrameModel
            }
        }
        Item {
            id: parkbrakeDtcPage
            Label {
                id: parkbarkeDtcLbl
                text: qsTr("Parkbrake DTC")+ tr.str
                font.pointSize: 20
                color: "white"
                anchors.top: parent.top
                anchors.topMargin: pageIndicator.y + pageIndicator.height
            }
            TableView {
                id: parkbrakeDtcList
                anchors.top: parkbarkeDtcLbl.bottom
                anchors.topMargin: 10
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 10
                width: 650
                model: dtcParkbrakeModel
                TableViewColumn {
                    role: "code"
                    title: qsTr("Code")+ tr.str
                    width: 70
                }
                TableViewColumn {
                    role: "desc"
                    title: qsTr("Description")+ tr.str
                    width: 400
                }
                TableViewColumn {
                    role: "occurrences"
                    title: qsTr("Occurrences")+ tr.str
                    width: 70
                }
                TableViewColumn {
                    role: "time"
                    title: qsTr("Time from startup")+ tr.str
                    width: 70
                }
            }
            Button {
                id: btnFreezeFrameParkbrake
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.rightMargin: 10
                text: qsTr("FreezeFrame") + tr.str
                onClicked: {
                    var dtc = parkbrakeNode.getDtc(parkbrakeDtcList.currentRow);
                    if (dtc) {
                        parkbrakeNode.fetchFreezeFrame(dtc.storedNr, function(ff){
                            freezeFrameModel.setFreezeFrame(ff);
                            dlgDtcCode.text = dtc.dtcCodeStr;
                            dlgFreezeFrame.open();
                        });
                    }
                }
            }
        }
    }

    // --------FreezeFrame dialog --------------------

    Dialog {
        id: dlgFreezeFrame
        width: 600
        height: 300
        contentItem: Item {
            Label {
                id: dlgHeadLine
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                text: qsTr("Freeze frame data for DTC") + tr.str
            }
            Label {
                id: dlgDtcCode
                anchors.top: dlgHeadLine.top
                anchors.left: dlgHeadLine.right
                anchors.leftMargin: 5

            }

            Button {
                id: btnOk
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                text: qsTr("OK") + tr.str
                onClicked: dlgFreezeFrame.close()
            }

            TableView {
                id: freezeFrameList
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.top: btnOk.bottom
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                model: freezeFrameModel
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
            }
        }

    }

    // ------- parkbrake model ------------
    ListModel {
        id: dtcParkbrakeModel
        function rebuild() {
            clear();
            for (var i = 0; i < parkbrakeNode.dtcCount; ++i) {
                var dtc = parkbrakeNode.getDtc(i);
                append({code: dtc.dtcCodeStr, desc: dtc.dtcDescription, occurrences: dtc.occurences,
                        time: dtc.timeSinceStartup});
            }
        }
        Component.onCompleted: rebuild()
    }

    Connections {
        target: parkbrakeNode
        onDtcArrived: dtcParkbrakeModel.rebuild();
    }
    Connections {
        target: parkbrakeNode
        onDtcsCleared: dtcParkbrakeModel.clear();
    }
    // ---------- end parkbrake model -------------

    // -----------freezeFrame model ---------------

    ListModel {
        id: freezeFrameModel
        function setFreezeFrame(ff) {
            // this function gets called from c++ when we after all freezeFrames has arrived
            clear();
            for (var i = 0; i < ff.pidCount; ++i) {
                var pid = ff.getPid(i);
                append({ name: pid.key, value: pid.valueStr + pid.unit });
            }
        }
    }

    // ----------- end freezeFrame model ----------


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
