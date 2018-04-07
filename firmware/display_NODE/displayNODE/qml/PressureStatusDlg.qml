import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import mummesoft 0.1
import "helpers.js" as Helpers

MessageDialog {
    id: root
    title: qsTr("Airpressure")+ tr.str
    standardButtons: StandardButton.Ok
    function pidGet(pidStr) {
        var pid = suspensionNode.getPid(qsTr(pidStr) + tr.str);
        if (pid)
            return pid.valueInt +  " " + pid.unit;
        return "";
    }
    contentItem: Rectangle {
        color: "lightskyblue"
        GridLayout {
            columns: 2
            Layout.margins: 10
            Image {
                source: "qrc:/images/gauge.svg"
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.leftMargin: 10
                Layout.rowSpan: 2
            }

            Label {
                text: qsTr("Airpressure status")+ tr.str
                font.bold: true
            }
            TableView {
                id: pressurePIDs
                width: 400
                height: 200
                Layout.margins: 10
                model: pressureDlgModel
                TableViewColumn {
                    role: "name"
                    title: qsTr("Name") + tr.str
                    width: 120
                }
                TableViewColumn {
                    role: "value"
                    title: qsTr("Value") + tr.str
                    width: 70
                }
            }
            Item {}
            Button {
                anchors.right: pressurePIDs.right
                anchors.top: pressurePIDs.bottom
                anchors.topMargin: 10
                Text {
                    anchors.centerIn: parent
                    text: qsTr("OK")+ tr.str
                }
                onClicked: root.close()
            }
        }
        Component.onCompleted: {
            root.width = 300;
            root.height = 225;
        }
    }
}
