import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers

TabBar {
    id: tabBar
    readonly property string name: "DiagHeader"
    property StackView view
    property ApplicationWindow globalApp
    height: 45
    TabButton {
        id: dtcTabBtn
        text: qsTr("Trouble codes")+ tr.str
        onClicked: Helpers.pushPage("DiagDTC");
    }
    TabButton {
        id: paramTabBtn
        text: qsTr("Parameters")+ tr.str
        onClicked: Helpers.pushPage("DiagParam");
    }
    TabButton {
        id: activateTabBtn
        text: qsTr("Activate")+ tr.str
        onClicked: Helpers.pushPage("DiagActivation");
    }
}
