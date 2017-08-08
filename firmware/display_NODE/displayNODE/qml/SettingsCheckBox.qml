import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers

CheckBox {
    id: root
    property int settingsIndex: 255

    onCheckedChanged: {
        function callbackSet(ok) {
            if (!ok) {
                var notifier = Helpers.globalApp().notifier
                notifier.setContent("qrc:/images/error.svg",
                                    qsTr("Error saving!"),
                                    qsTr("A error ourred during saving setting"))
                notifier.errorSnd.play();
            }
        }
        var vlu = checked ? 1 : 0;
        parkbrakeNode.setSetting(settingsIndex, vlu, callbackSet);
    }

    Component.onCompleted: {
        function callbackGet(vlu) {
            checked = vlu > 0  ? true: false;
        }

        parkbrakeNode.fetchSetting(settingsIndex, callbackGet);
    }
}

