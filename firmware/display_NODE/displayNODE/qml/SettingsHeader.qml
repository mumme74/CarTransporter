import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers

TabBar {
    id: tabBar
    readonly property string name: "SettingsTabs"
    property StackView view
    property ApplicationWindow globalApp
    height: 45
    TabButton {
        id: dtcTabBtn
        text: qsTr("Display")+ tr.str
        onClicked: Helpers.pushPage("SettingsDisplay");
    }
    TabButton {
        id: paramTabBtn
        text: qsTr("Suspension")+ tr.str
        onClicked: Helpers.pushPage("SettingsSuspension");
    }
    TabButton {
        id: activateTabBtn
        text: qsTr("Parkbrake")+ tr.str
        onClicked: Helpers.pushPage("SettingsParkbrake");
    }
}
