import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

Button {
    id: iconBtn
    opacity: 0.5
    property real restScale: 0.6
    property string source: ""
    property string btnText: ""
    signal clicked

    transform: Scale { id: iconBtnTransform;
                        origin.x: 30; origin.y: 30;
                        property real scale: restScale
                        xScale: scale; yScale: scale; }
    Image {
        id: icon
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.verticalCenter: parent.verticalCenter
        source: parent.source
        fillMode: Image.PreserveAspectFit
        Component.onCompleted: {
            parent.width = icon.width +20;
            if (textMetrics.advanceWidth)
                parent.width += textMetrics.advanceWidth + 20
            parent.height = icon.height +10
        }
    }

    Text {
        id: text
        text: btnText
        color: "white"
        font.pointSize: 20
        anchors.left: icon.right
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
    }

    TextMetrics {
        id: textMetrics
        font.family: text.font.family
        font.pointSize: 20
        text: btnText
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: ParallelAnimation {
            OpacityAnimator {
                target: iconBtn
                from: iconBtn.opacity
                to: iconBtn.enabled ? 1.0 : 0.5
                duration: 300
            }
            NumberAnimation {
                target: iconBtnTransform
                property: "scale"
                from: iconBtnTransform.scale
                to: 1.0
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
        onExited: ParallelAnimation {
            OpacityAnimator {
                target: iconBtn
                from: iconBtn.opacity
                to: 0.5
                duration: 300
            }
            NumberAnimation {
                target: iconBtnTransform
                property: "scale"
                from: iconBtnTransform.scale
                to: restScale
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
        onClicked: iconBtn.clicked()
    }
}
