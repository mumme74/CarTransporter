import QtQuick 2.7
import QtQuick.Controls 2.0
import QtMultimedia 5.8

Rectangle {
    id: notifierWgt
    color: "#f9c5a2"
    radius: 10
    anchors.centerIn: parent
    visible: true
    opacity: 0.0
    width: Math.max(notifierText.width, notifierHeadline.width) +
                    notifierIcon.width + closeIcon.width + 30
    height: Math.max(notifierIcon.height,
                     notifierHeadline.height + notifierText.height + 40)
    Row {
        anchors.centerIn: parent
        spacing: 10
        Image {
            id: notifierIcon
        }
        Column {
            Label {
                id: notifierHeadline
                color: "#00032d"
                font.pointSize: 20
            }
            Label {
                id: notifierText
                color: "#00032d"
                font.pointSize: 14
            }
        }
    }

    // our closing cross
    Rectangle {
        id: closeBackground
        width: closeIcon.width + 6
        height: width
        radius: width / 2
        x: notifierWgt.width - width - 7
        y: 7
        z: 1
        color: "gray"
        border.color: "darkgray"
        border.width: 1
        opacity: 0.5
        Image {
            id: closeIcon
            source: "qrc:/images/delete.svg"
            width: 10
            height: 10
            x: 3
            y: 3
            opacity: 0.5
        }
        MouseArea {
            anchors.fill: closeBackground
            hoverEnabled: true
            onClicked: hideNotification();
            onContainsMouseChanged: {
                if (containsMouse) {
                    closeIcon.opacity = 1.0;
                    closeBackground.opacity = 1.0;
                } else {
                    closeIcon.opacity = 0.5;
                    closeBackground.opacity = 0.5;
                }
            }
        }

    }


    Timer {
        id: notifierHideTmr
        repeat: false
        interval: 6000
        running: false
        onTriggered: {
            notifierAnimator.from = 1.0
            notifierAnimator.to = 0.0
            notifierAnimator.running = true
        }
    }
    OpacityAnimator {
        id: notifierAnimator
        target: notifierWgt
        from: 0.0
        to: 1.0
        duration: 500
    }
    SoundEffect {
        id: beepSnd
        source: "qrc:/sounds/Beep.wav"
    }
    SoundEffect {
        id: bleepSnd
        source: "qrc:/sounds/Bleep.wav"
    }
    SoundEffect {
        id: chimeSnd
        source: "qrc:/sounds/Chime.wav"
    }
    SoundEffect {
        id: errorSnd
        source: "qrc:/sounds/Error.wav"
    }

    function setContent(iconStr, headLine, text) {
        notifierIcon.source = iconStr
        notifierHeadline.text = headLine
        notifierText.text = text
    }
    function showNotification() {
        visible = true
        if (notifierWgt.opacity !== 0.0) {
            notifierHideTmr.restart();
        }

        notifierAnimator.to = 1.0
        notifierAnimator.from = notifierWgt.opacity
        notifierAnimator.running = true
        notifierHideTmr.start();

    }
    function hideNotification() {
        notifierHideTmr.stop();
        notifierAnimator.stop();
        notifierAnimator.to = 0.0
        notifierAnimator.running = true

    }

    Connections {
        target: parkbrakeNode
        onNewDtcSet: {
           notifierIcon.source = "qrc:/images/diagnostic.svg";
           notifierHeadline.text = qsTr("Parkbrake DTC set!") + tr.str;
           notifierText.text = dtc.dtcCodeStr + " " + dtc.dtcDescription;
           notifierWgt.showNotification();
            errorSnd.play()
        }
    }
    Connections {
        target: parkbrakeNode
        onUserErrorIgnOff: {
           notifierIcon.source = "qrc:/images/ignition.svg";
           notifierHeadline.text = qsTr("Ignition must be on!") + tr.str;
           notifierText.text = qsTr("Turn on ignition") + tr.str;
           notifierWgt.showNotification();
           beepSnd.play()
        }
    }

    Connections {
        target: parkbrakeNode
        onUserErrorBrakeOff: {
           notifierIcon.source = "qrc:/images/brake_pedal.svg";
           notifierHeadline.text = qsTr("Brake pedal!") + tr.str;
           notifierText.text = qsTr("Must bress brake") + tr.str;
           notifierWgt.showNotification();
           bleepSnd.play()
        }
    }

    Connections {
        target: parkbrakeNode
        onUserErrorBtnInvOff: {
           notifierIcon.source = "qrc:/images/parkbrake.svg";
           notifierHeadline.text = qsTr("Hold release button!") + tr.str;
           notifierText.text = qsTr("Must hold parkbrake release button") + tr.str;
           notifierWgt.showNotification();
           bleepSnd.play()
        }
    }

    Connections {
        target: parkbrakeNode
        onServiceModeChanged: {
           notifierIcon.source = "qrc:/images/service.svg";
           notifierHeadline.text = qsTr("Servicemode") + tr.str;
           if (service)
               notifierText.text = qsTr("Service mode activated!") + tr.str;
           else
               notifierText.text = qsTr("Service mode deactivated!") + tr.str;

           notifierWgt.showNotification();
           bleepSnd.play()
        }
    }

    Connections {
        target: suspensionNode
        onUserErrorHeightNonValidState: {
            setContent("qrc:/images/warning.svg",
                       qsTr("Non valid height"),
                       qsTr("You tried a non valid height change"));
            notifierWgt.showNotification();
            bleepSnd.play()
        }
    }

    Connections {
        target: suspensionNode
        onUserErrorSuckingNotAllowed: {
            setContent("qrc:/images/warning.svg",
                                    qsTr("Not allowed"),
                                    qsTr("You can't lift rear wheels\n" +
                                        "Perhaps you forgot lowering first\n" +
                                        "Or weight is to high"));
            notifierWgt.showNotification();
            bleepSnd.play()
        }
    }
}
