.pragma library
.import QtQuick 2.2 as QtQuick2
.import "helpers.js" as Helpers

var DataTypes = {
    type_u8: 8,
    type_u16: 16,
    type_u32: 32,
    type_float: 65
};

// for setting controls, SettingsSpinbox, SettingsCheckbox etc
function setSetting(node, dataType, settingsIndex, value) {
    function callbackSet(ok) {
        if (!ok) {
            var notifier = Helpers.globalApp().notifier
            notifier.setContent("qrc:/images/error.svg",
                                qsTr("Error saving!"),
                                qsTr("A error ourred during saving the setting"))
            notifier.errorSnd.play();
        }
    }

    var fn;
    switch (dataType) {
    case DataTypes.type_u8:
        fn = node.setSettingUInt8;
        break;
    case DataTypes.type_u32:
        fn = node.setSettingUInt32;
        break;
    case DataTypes.type_float:
        fn = node.setSettingFloat;
        break;
    case DataTypes.type_u16: // fallthrough
    default:
        fn = node.setSettingUInt16;
    }

    if (fn)
        return fn(settingsIndex, value, callbackSet);

    return false;
}
