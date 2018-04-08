import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import "helpers.js" as Helpers

MessageDialog {
    id: root
    title: qsTr("Weight")+ tr.str
    property string carState: "NormalState"
    standardButtons: StandardButton.Ok
    contentItem: Rectangle {
        color: "lightskyblue"
        implicitWidth: 600
        implicitHeight: 200
        GridLayout {
            columns: 2
            Layout.margins: 10
            Image {
                source: "qrc:/images/weight.svg"
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.leftMargin: 10
                Layout.rowSpan: 3
            }

            Label {
                id: headLine
                text: qsTr("Weight calculation") + tr.str
                font.bold: true
                Layout.margins: 10
            }
            PID_Label {
                id: weightLbl
                Layout.margins: 10
                pid: suspensionNode.getPid(qsTr("Load_weight") + tr.str);
                format: function (pid) {
                    return pid.valueStr + pid.unit;
                }
            }
            Item {
                // gridlayout workaround
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
        width = 280;
        height = 100;
    }

    onVisibleChanged: {
        if (visible) {
            if (carState === "NormalState") {
                headLine.text = qsTr("Weight calculation") + tr.str
                headLine.color = "white";
            } else {
                headLine.text = qsTr("Weight not available!\nMust be at normal height")
                headLine.color = "red";
            }
        }
    }
}
