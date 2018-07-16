import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers
import "settings.js" as Settings

SpinBox {
    id: root
    property int settingsIndex: 255
    property int dataType: Settings.DataTypes.type_u16
    property variant node
    property bool warnDeadNodeInitialNode: false

    Timer {
        id: delay
        interval: 800
        onTriggered: {
            if (!Settings.setSetting(node, dataType, settingsIndex, value))
                Helpers.notifyCanIsOff();
        }
    }

    Timer {
        id: deadNodeWarn
        interval: 500
        onTriggered: {
            Helpers.getNotifier().setContent("qrc:/images/error.svg",
                                             qsTr("Network error"),
                                             qsTr("No response from SuspensionNode"));
            Helpers.getNotifier().showNotification();
        }
    }

    onValueModified: {
        delay.restart()
    }

    Component.onCompleted: {
        function callbackGet(vlu) {
            value = vlu;
            deadNodeWarn.stop();
        }

        if (warnDeadNodeInitialNode)
            deadNodeWarn.start();

        node.fetchSetting(settingsIndex, callbackGet);
    }
}
