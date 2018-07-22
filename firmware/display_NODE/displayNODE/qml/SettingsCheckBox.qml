import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers
import "settings.js" as Settings

CheckBox {
    id: root
    property int settingsIndex: 255
    property int dataType: Settings.DataTypes.type_u16
    property variant node
    property bool _blockSave: false

    onCheckedChanged: {
        if (!_blockSave) {
            var vlu = checked ? 1 : 0;
            if (!Settings.setSetting(node, dataType, settingsIndex, vlu))
                Helpers.notifyCanIsOff();
        }
        _blockSave = false;
    }

    Component.onCompleted: {
        function callbackGet(vlu) {
            _blockSave = true;
            checked = vlu > 0  ? true: false;
        }

        node.fetchSetting(settingsIndex, callbackGet);
    }
}

