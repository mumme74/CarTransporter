import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers

SpinBox {
    id: root
    property int settingsIndex: 255

    onValueModified: {
        function callbackSet(ok) {
            if (!ok) {
                var notifier = Helpers.globalApp().notifier
                notifier.setContent("qrc:/images/error.svg",
                                    qsTr("Error saving!"),
                                    qsTr("A error ourred during saving setting"))
                notifier.errorSnd.play();
            }
        }

        parkbrakeNode.setSetting(settingsIndex, value, callbackSet);
    }

    Component.onCompleted: {
        function callbackGet(vlu) {
            value = vlu;
        }

        parkbrakeNode.fetchSetting(settingsIndex, callbackGet);
    }
}
