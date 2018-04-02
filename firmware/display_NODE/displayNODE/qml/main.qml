import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "helpers.js" as Helpers


ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 480
    title: qsTr("DisplayNode")+ tr.str

    Component.onCompleted: Helpers.init(root, mainView, forwardBtn, notifier);

    StackView {
        id: mainView
        anchors.fill: parent
        initialItem: mainPage
        onCurrentItemChanged: Helpers.checkForward();
    }
    MainPage {
        id: mainPage
    }

    DiagDTC {
        id: diagnosticPage
        visible: false
    }

    SettingsDisplay {
        id: settingsPage
        visible: false
    }

    MyNotifier {
        id: notifier
    }

    footer: Item {
        //anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50
        IconButton {
            id: homeBtn
            source: "qrc:/images/home.svg"
            visible: mainView.depth > 1
            anchors.left: parent.left
            onClicked: {
                while (mainView.depth > 1)
                    mainView.pop();
            }
        }

        IconButton {
            id: backBtn
            source: "qrc:/images/return_arrow.svg"
            btnText: qsTr("Back")+ tr.str
            onClicked: Helpers.back();
            visible: mainView.depth > 1
            anchors.left: homeBtn.right
            anchors.leftMargin: 5
        }
        IconButton {
            id: forwardBtn
            source: "qrc:/images/forward_arrow.svg"
            btnText: qsTr("Forward")+ tr.str
            onClicked: Helpers.forward();
            visible: mainView.depth > 1
            anchors.left: backBtn.right
            anchors.leftMargin: 5
        }
        IconButton {
            id: diagnosticsBtn
            source: "qrc:/images/diagnostic.svg"
            btnText: qsTr("Diagnostics")+ tr.str
            anchors.left: forwardBtn.right
            anchors.leftMargin: 10
            onClicked: {
                mainView.pop(mainView.find(function(itm) { return itm === diagnosticPage; }));
                mainView.push(diagnosticPage);
            }
        }

        IconButton {
            id: settingsBtn
            source: "qrc:/images/settings.svg"
            btnText: qsTr("Settings")+ tr.str
            anchors.right: parent.right
            onClicked: {
                mainView.pop(mainView.find(function(itm){return itm === settingsPage; }));
                mainView.push(settingsPage)
            }
        }
    }
}
