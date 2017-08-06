import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

Item {
    id: mainPage
    Item {
        id: car
        anchors.fill:parent

        // this is the state we are currently at
        states: [
            State {
                name: "low"
                PropertyChanges {
                        target: rot
                        angle: 2
                }
            },

            State {
                name: ""
                PropertyChanges {
                        target: rot
                        angle: 0
                }
            },

            State {
                name: "top"
                PropertyChanges {
                        target: rot
                        angle: -2
                }
            }
        ]

        // this state is to show that we are moving
        Item {
            id: carMovingState
            anchors.fill: parent
            states: [
                State {
                    name: ""
                    PropertyChanges {
                        target: upDownArrowAnimation
                        running: false
                    }
                },
                State {
                    name: "movingUp"
                    changes: [
                        PropertyChanges {
                            target: upDownArrow;
                            source: "qrc:/images/arrow_up.svg"
                        },
                        PropertyChanges {
                            target: upDownArrowAnimation
                            running: true
                        }
                    ]
                },
                State {
                    name: "movingDown"
                    changes: [
                        PropertyChanges {
                            target: upDownArrow
                            source: "qrc:/images/arrow_down.svg"
                        },
                        PropertyChanges {
                            target: upDownArrowAnimation
                            running: true
                        }
                    ]
                }
            ]

        }



        // lift rear wheel
        onStateChanged: {
            if (rearWheel.state == "lifted") {
                switch (car.state) {
                case "low":
                    rotRearWheel.angle = 0;
                    return;
                case "top":
                    rotRearWheel.angle = -4;
                    return;
                default:
                    rotRearWheel.angle = -2;
                }
            } else {
                rotRearWheel.angle = 0;
            }
        }

        Image {
            id: carImage
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            source: "qrc:/images/car.svg"
            transform: Rotation{ id:rot
                          origin.x: 107; origin.y: 157;
                          angle: 0
                          Behavior on angle { SmoothedAnimation { velocity: 2.5 }}
            }


            Image {
                id: upDownArrow
                opacity: 0.0
                anchors.left: carImage.left
                anchors.leftMargin: 450
                anchors.bottom: carImage.bottom
                anchors.bottomMargin: 85

                source: "qrc:/images/arrow_down.svg"

                SequentialAnimation {
                    id:upDownArrowAnimation
                    running: false
                    loops: Animation.Infinite
                    OpacityAnimator {
                        target: upDownArrow;
                        from: upDownArrow.opacity
                        to: 0.7;
                        duration: 500
                    }
                    OpacityAnimator {
                        target: upDownArrow;
                        from: upDownArrow.opacity
                        to: 0.0;
                        duration: 500
                    }
                    onRunningChanged: {
                        console.log("changed", running)
                    }
                }
            }
        }

        Image {
            id: frontWheel
            anchors.left: carImage.left
            anchors.leftMargin: 438
            anchors.bottom: carImage.bottom
            source: "qrc:/images/wheel.svg"
        }

        Image {
            id: rearWheel
            anchors.left: carImage.left
            anchors.leftMargin: 508
            anchors.bottom: carImage.bottom
            source: "qrc:/images/wheel.svg"
            transform: Rotation{
                        id:rotRearWheel
                                 origin.x: 107 - 508; origin.y: 157-65;
                                 angle: 0
                                 Behavior on angle { SmoothedAnimation { velocity: 2.5 }}
                   }
            states: [
                State { name: "lifted" }
            ]

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (car.state == "low") {
                        rearWheel.state = rearWheel.state == "lifted" ? "" : "lifted";
                        rearWheelLiftArrow.opacity = rearWheel.state == "lifted" ?  0.6 : 0.0
                    } else {
                        // display warning dialog
                        carLiftedDialog.open();
                    }
                }
            }

            Image {
                id:rearWheelLiftArrow
                anchors.centerIn: rearWheel
                source: "qrc:/images/arrow_up.svg"
                transform: Scale { origin.x: 30; origin.y: 30; xScale: 0.5; yScale: 0.5; }
                opacity: 0.0
                Behavior on opacity { SmoothedAnimation { velocity: 0.9 }}
            }
        }


        Image {
            id: ground
            anchors.left: carImage.left
            anchors.top: carImage.bottom
            anchors.topMargin: -7
            source: "qrc:/images/ground.svg"
        }


        Slider {
            id: heightSlider
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            from: 0
            to: 2
            value: 1
            orientation: Qt.Vertical
            stepSize: 1
            snapMode: "SnapAlways"
            onMoved: {
                console.log(value);
                if(value == 0) {
                    car.state = "low";
                    carMovingState.state = "movingDown"
                } else if (value == 1) {
                    carMovingState.state = car.state == "low" ? "movingUp" : "movingDown";
                    car.state = ""; // normal
                } else if (value == 2) {
                    car.state = "top";
                    carMovingState.state = "movingUp"
                }
            }
        }


        MessageDialog {
            id: carLiftedDialog
            title: qsTr("Forbidden!")
            text: qsTr("Can't operate rear wheels when not in low state");
        }

    } // end car



    IconButton {
        id: compressorBtn
        source: "qrc:/images/gauge.svg"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 10
    }

    IconButton {
        id: weightBtn
        enabled: car.state == ""
        source: "qrc:/images/weight.svg"
        anchors.right: compressorBtn.left
        anchors.top: parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 10
        onClicked: {
            weightDlg.visible ? weightDlg.close() : weightDlg.open()
        }
    }

    IconButton {
        id: parkbrakeBtn
        source: "qrc:/images/parkbrake.svg"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 10
        onClicked: {
            parkbrakeDlg.visible ? parkbrakeDlg.close() : parkbrakeDlg.open()
        }
    }

    MessageDialog {
        id: weightDlg
        title: qsTr("Weight")
        standardButtons: StandardButton.Ok
        contentItem: Rectangle {
            color: "lightskyblue"
            implicitWidth: 400
            implicitHeight: 100
            GridLayout {
                columns: 2
                Layout.margins: 10
                Image {
                    source: "qrc:/images/weight.svg"
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    anchors.leftMargin: 10
                    Layout.rowSpan: 3
                }

                Label {
                    text: qsTr("Weight calculation")
                    font.bold: true
                    Layout.margins: 10
                }
                Label {
                    text: "1000kg(TODO implement)"
                    Layout.margins: 10
                }
                Item {
                    // gridlayout workaround
                }
            }
            Button {
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                Text {
                    anchors.centerIn: parent
                    text: qsTr("OK")
                }
                onClicked: weightDlg.close()
            }
        }
    }

    MessageDialog {
        id: parkbrakeDlg
        title: qsTr("Parkbrake")
        standardButtons: StandardButton.Ok
        contentItem: Rectangle {
            color: "lightskyblue"
            implicitWidth: 400
            implicitHeight: 150
            GridLayout {
                columns: 2
                Layout.margins: 10
                Image {
                    source: "qrc:/images/parkbrake.svg"
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    anchors.leftMargin: 10
                    Layout.rowSpan: 3
                }

                Label {
                    text: qsTr("Parkbrake status")
                    font.bold: true
                    Layout.margins: 10
                }
                Label {
                    text: "Activated(TODO implement)"
                    Layout.margins: 10
                }
                Item {
                    // gridlayout workaround
                }
            }
            Button {
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                Text {
                    anchors.centerIn: parent
                    text: qsTr("OK")
                }
                onClicked: parkbrakeDlg.close()
            }
        }
    }
}
