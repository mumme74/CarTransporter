import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers
import "settings.js" as Settings

SpinBox {
    id: root
    property int settingsIndex: 255
    property int dataType: Settings.DataTypes.type_u16
    property variant node

    Timer {
        id: delay
        interval: 800
        onTriggered: {
            if (!Settings.setSetting(node, dataType, settingsIndex, value))
                Helpers.notifyCanIsOff();
        }
    }

    onValueModified: {
        delay.restart()
    }

    Component.onCompleted: {
        function callbackGet(vlu) {
            value = vlu;
        }

        node.fetchSetting(settingsIndex, callbackGet);
    }
}
