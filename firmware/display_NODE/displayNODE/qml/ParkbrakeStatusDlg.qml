import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import "helpers.js" as Helpers

Dialog {
    id: root
    title: qsTr("Parkbrake")+ tr.str
    standardButtons: StandardButton.Ok
    contentItem: Rectangle {
        color: "lightskyblue"
        implicitWidth: 300
        implicitHeight: 100
        GridLayout {
            columns: 2
            Layout.margins: 10
            Image {
                source: "qrc:/images/parkbrake.svg"
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.leftMargin: 10
                Layout.rowSpan: 3
            }

            Label {
                text: qsTr("Parkbrake status") + tr.str
                font.bold: true
            }
            Grid {
                columns: 2
                Layout.margins: 10
                spacing: 10
                padding: 5

                PID_Label {
                    id: lfBrake
                    pid: parkbrakeNode.getPid(qsTr("LeftFront_state") + tr.str);
                    format: function (pid) {
                        return qsTr("LF:") + tr.str + pid.valueStr;
                    }
                }
                PID_Label {
                    id: rfBrake
                    pid: parkbrakeNode.getPid(qsTr("RightFront_state") + tr.str);
                    format: function (pid) {
                        return qsTr("RF:") + tr.str + pid.valueStr;
                    }
                }
                PID_Label {
                    id: lrBrake
                    pid: parkbrakeNode.getPid(qsTr("LeftRear_state") + tr.str);
                    format: function (pid) {
                        return qsTr("LR:") + tr.str + pid.valueStr;
                    }
                }

                PID_Label {
                    id: rrBrake
                    pid: parkbrakeNode.getPid(qsTr("RightRear_state") + tr.str);
                    format: function (pid) {
                        return qsTr("RR:") + tr.str + pid.valueStr;
                    }
                }
            }
        }
        Button {
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            Text {
                anchors.centerIn: parent
                text: qsTr("OK")+ tr.str
            }
            onClicked: root.close()
        }
    }
    Component.onCompleted: {
        root.width = 300
        root.height = 150
    }

}
