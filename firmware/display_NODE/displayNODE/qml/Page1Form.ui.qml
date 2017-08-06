import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: item1

    RowLayout {
        anchors.horizontalCenterOffset: -129
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 138
        anchors.top: parent.top
    }

    Slider {
        id: heightSlider
        x: 723
        y: 238
        width: 48
        height: 200
        smooth: false
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 20
        stepSize: 1
        to: 3
        spacing: 1
        antialiasing: true
        transformOrigin: Item.BottomRight
        orientation: Qt.Vertical
        value: 1
    }
}
