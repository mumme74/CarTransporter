import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers
import "settings.js" as Settings

SpinBox {
    id: spinbox
    property real realFrom: 0.0
    property real realTo: 100.0
    property real realStepSize: 0.1
    property int decimals: 2
    property real realValue: 1.0

    from: realFrom * 100
    value: 100 * realValue
    to: realTo * 100
    stepSize: 100 * realStepSize

    property int settingsIndex: 255
    property variant node

    Timer {
        id: delay
        interval: 800
        onTriggered: {
            if (!Settings.setSetting(node, Settings.DataTypes.type_float,
                                settingsIndex, realValue))
            {
                Helpers.notifyCanIsOff();
            }
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

    validator: DoubleValidator {
        bottom: Math.min(spinbox.from, spinbox.to)
        top:  Math.max(spinbox.from, spinbox.to)
    }

    textFromValue: function(value, locale) {
        return Number(value / 100).toLocaleString(locale, 'f', spinbox.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * 100
    }
}
