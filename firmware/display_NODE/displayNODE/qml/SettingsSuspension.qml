import QtQuick 2.7
import QtQuick.Controls 2.2

import "helpers.js" as Helpers

Item {
    id: root

    onVisibleChanged: {
        Helpers.switchToSettingsHeader(root);
    }

    Label {
        id: pageHeadline
        text: qsTr("Suspension node")+ tr.str
        color: "white"
        font.pointSize: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
    }

}
