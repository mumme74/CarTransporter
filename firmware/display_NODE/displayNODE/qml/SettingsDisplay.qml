import QtQuick 2.7
import QtQuick.Controls 2.2
import "helpers.js" as Helpers


Item {
    id: root


    onVisibleChanged: {
        Helpers.switchToSettingsHeader(root);
    }

    Label {
        id: pageHeadline
        text: qsTr("Display node")+ tr.str
        color: "white"
        font.pointSize: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    Row {
        anchors.top: pageHeadline.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        ComboBox {
            id: languageBox
            textRole: "text"
            model: ListModel {
                id: languageItems
                function rebuild(){
                    clear();
                    append({ text: qsTr("English"), lang: "en"});
                    append({ text: qsTr("Swedish"), lang: "sv"});
                    var lang = tr.getLang();
                    for (var i = 0; i < languageItems.count; ++i) {
                        if (languageItems.get(i).lang == lang) {
                            languageBox.currentIndex = i;
                            break;
                        }
                    }
                }
                Component.onCompleted: rebuild();
            }
            width: 150
            onCurrentIndexChanged: {
                var itm = languageItems.get(currentIndex);
                if (!itm)
                    return;
                var lang = itm.lang;
                if (lang != tr.getLang())
                    tr.selectLanguage(lang)
            }
            Connections {
                target: tr
                onLanguageChanged: languageItems.rebuild();
            }
        }

    }
}
