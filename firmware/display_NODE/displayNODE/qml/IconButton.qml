import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

Image {
    id: iconBtn
    opacity: 0.5
    property real restScale: 0.5
    signal clicked

    transform: Scale { id: iconBtnTransform;
                        origin.x: 30; origin.y: 30;
                        property real scale: restScale
                        xScale: scale; yScale: scale; }

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
