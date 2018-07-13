import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import "helpers.js" as Helpers
import mummesoft 0.1

Item {
    id: root
    IconButton {
        id: clearBtn
        anchors.left: root.left
        anchors.leftMargin: 5
        anchors.top: root.top
        anchors.topMargin: 5
        btnText: qsTr("Clear") + tr.str
        source: "qrc:/images/delete.svg"

        onClicked: {
            console.log("clearing CanErrorsModel");
            var cnt = canErrorsModel.clearAll();
            console.log("cleared: ", cnt);
        }
    }
    Label {
        id: headerLbl
        text: qsTr("CAN errors") + tr.str
        font.pointSize: 20
        color: "white"
        anchors.top: root.top
        anchors.topMargin: pageIndicator.y + pageIndicator.height
        anchors.left: clearBtn.right
        anchors.leftMargin: 10
    }

    TableView {
        id: canErrorsTbl
        anchors.top: headerLbl.bottom
        anchors.topMargin: 10
        anchors.bottom: root.bottom
        anchors.right: root.right
        anchors.rightMargin: 10
        anchors.left: root.left
        anchors.leftMargin: 10
        model: canErrorsModel
        TableViewColumn {
            role: "type"
            title: qsTr("Type") + tr.str
            width: 200
        }
        TableViewColumn {
            role: "description"
            title: qsTr("Description") + tr.str
            width: 600
        }
    }

}
