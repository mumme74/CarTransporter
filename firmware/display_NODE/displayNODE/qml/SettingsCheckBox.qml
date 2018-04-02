import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers
import "settings.js" as Settings

CheckBox {
    id: root
    property int settingsIndex: 255
    property int dataType: Settings.DataTypes.type_u16
    property variant node

    onCheckedChanged: {
        var vlu = checked ? 1 : 0;
        Settings.setSetting(node, dataType, settingsIndex, vlu);
    }

    Component.onCompleted: {
        function callbackGet(vlu) {
            checked = vlu > 0  ? true: false;
        }

        node.fetchSetting(settingsIndex, callbackGet);
    }
}

