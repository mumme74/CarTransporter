import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import "helpers.js" as Helpers
import mummesoft 0.1

Item {
    id: root
    property var node
    property var pidsListSelectFrom
    IconButton {
        id: selectBtn
        anchors.left: root.left
        anchors.leftMargin: 5
        anchors.verticalCenter: root.verticalCenter
        anchors.verticalCenterOffset: -30
        btnText: ">>"
        onClicked: {
            var pid = node.getPid(pidsListSelectFrom.currentRow);
            if (pid)
                canPidsModel.addPid(pid);

            console.log("add", pid, pid.key);
        }
    }

    IconButton {
        id: deSelectBtn
        anchors.left: root.left
        anchors.leftMargin: 5
        anchors.verticalCenter: root.verticalCenter
        anchors.verticalCenterOffset: 30
        btnText: ">!<"
        onClicked: {
            var pid = canPidsModel.getPid(selectedPIDsList.currentRow);
            canPidsModel.removePid(pid);
            if (pid) console.log(pid.key);
            console.log("remove", pid, pid.key)
        }
    }

    Label {
       id: selectedPIDsLbl
       text: qsTr("Selected PIDs")
       font.pointSize: 20
       color: "white"
       anchors.top: root.top
       anchors.topMargin: pageIndicator.y + pageIndicator.height
       anchors.right: root.right
       anchors.rightMargin: 10
    }
    TableView {
        id: selectedPIDsList
        anchors.top: selectedPIDsLbl.bottom
        anchors.topMargin: 10
        anchors.bottom: root.bottom
        anchors.right: root.right
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

}
