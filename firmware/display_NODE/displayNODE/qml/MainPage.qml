import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import "helpers.js" as Helpers

Page {
    id: mainPage

    Item {
        id: car
        anchors.fill:parent

        property string previousState: "NormalState"
        state: "NormalState"

        // this is the state we are currently at
        states: [
            State {
                name: "LowState"
                changes: [
                    PropertyChanges {
                        target: rot
                        angle: 2
                    },
                    PropertyChanges {
                        target: carMovingState
                        state: ""
                    }
                ]
            },

            State {
                name: "ToLowState"
                changes: [
                    PropertyChanges {
                        target: rot
                        angle: 2
                    },
                    PropertyChanges {
                        target: carMovingState
                        state: "movingDown"
                    }
                ]
            },

            State {
                name: "NormalState"
                changes: [
                    PropertyChanges {
                        target: rot
                        angle: 0
                    },
                    PropertyChanges {
                        target: carMovingState
                        state: ""
                    }
                ]
            },

            State {
                name: "ToNormalState"
                changes: [
                    PropertyChanges {
                        target: rot
                        angle: 0
                    },
                    PropertyChanges {
                        target: carMovingState
                        state: {
                            if (Helpers.carState === "LowState" ||
                                Helpers.carState === "ToLowState")
                                return "movingUp"
                            return "movingDown"
                        }
                    }
                ]
            },

            State {
                name: "HighState"
                changes: [
                    PropertyChanges {
                        target: rot
                        angle: -2
                    },
                    PropertyChanges {
                        target: carMovingState
                        state: ""
                    }
                ]
            },

            State {
                name: "ToHighState"
                changes: [
                    PropertyChanges {
                        target: rot
                        angle: -2
                    },
                    PropertyChanges {
                        target: carMovingState
                        state: "movingUp"
                    }
                ]
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
                case "ToLowState":
                case "LowState":
                    rotRearWheel.angle = 0;
                    return;
                case "ToHighState":
                case "HighState":
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
                    if (car.state == "LowState") {
                        rearWheel.state = rearWheel.state == "lifted" ? "" : "lifted";
                        rearWheelLiftArrow.opacity = rearWheel.state == "lifted" ?  0.6 : 0.0
                    } else {
                        // display warning dialog
                        Helpers.getNotifier().setContent("qrc:/images/warning.svg",
                                                         qsTr("Forbidden!")+ tr.str,
                                                         qsTr("Can't operate rear wheels when not in confirmed low state")+ tr.str);
                        Helpers.getNotifier().showNotification();
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

        Timer {
            id: networkTimeout
            interval: 2000
            onTriggered: {
                Helpers.getNotifier().showNotification();
            }
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
            function setState(vlu) {
                if (vlu === undefined)
                    vlu = value;

                switch (vlu) {
                case 0:
                    car.state = "ToLowState";
                    break;
                case 2:
                    car.state = "ToHighState";
                    break;
                case 1: // fallthrough
                default:
                    car.state = "ToNormalState"; // normal
                }
                // save until next state propertychange
                Helpers.carState = car.state;
                Helpers.getNotifier().setContent("qrc:/images/error.svg", qsTr("NetworkError"), qsTr("No resonse from CAN"))
                networkTimeout.start();
            }
            onMoved: {
                setState(value);
                var res = suspensionNode.setHeightState(car.state, function(res){
                    if (res === 0xAA)
                        networkTimeout.stop();
                });

                if (!res)
                    Helpers.notifyCanIsOff();
            }
        }

        Connections {
            target: suspensionNode
            onHeightStateChanged: {
                if (state === "UnknownState" ||
                    state === "ErrorState")
                {
                    Helpers.getNotifier().setContent("qrc:/images/error.svg",
                                                     "Wrong response from suspensionNode",
                                                     state);
                    Helpers.getNotifier().showNotification();
                    return;
                }

                car.state = state;
            }

        }

    } // end car

    onVisibleChanged: {
        if (visible) {
            Helpers.hideDiagHeader();
        }
    }

    Image {
        id: logo
        source: "qrc:/images/logo.svg"
        opacity: 0.8
        anchors.horizontalCenter: parent.horizontalCenter
        y: -40
        fillMode: Image.PreserveAspectFit
        //width: 150
        height: 120
    }

    header: Item {
        id: headerMain
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50
        IconButton {
            id: compressorBtn
            source: "qrc:/images/gauge.svg"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 10
            //anchors.topMargin: 10
            onClicked: {
                pressureDlg.visible ? pressureDlg.close() : pressureDlg.open();
            }
        }

        IconButton {
            id: weightBtn
            source: "qrc:/images/weight.svg"
            anchors.right: compressorBtn.left
            anchors.top: parent.top
            anchors.rightMargin: 10
            //anchors.topMargin: 10
            onClicked: {
                weightDlg.carState = car.state;
                weightDlg.visible ? weightDlg.close() : weightDlg.open()
            }
        }

        IconButton {
            id: parkbrakeBtn
            source: "qrc:/images/parkbrake.svg"
            opacity: {
                return (parkbrakeNode.getPid(qsTr("LeftFront_state") + tr.str).valueInt > 0 ||
                        parkbrakeNode.getPid(qsTr("RightFront_state") + tr.str).valueInt > 0 ||
                        parkbrakeNode.getPid(qsTr("LeftRear_state") + tr.str).valueInt > 0 ||
                        parkbrakeNode.getPid(qsTr("RightRear_state") + tr.str).valueInt > 0) ? 1.0 : restOpacity
            }
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.rightMargin: 10
            //anchors.topMargin: 10
            onClicked: {
                parkbrakeDlg.visible ? parkbrakeDlg.close() : parkbrakeDlg.open()
            }
        }

        Timer {
            id: clockTmr
            interval: 5000; running: true; repeat: true;
            onTriggered: clockBtn.timeUpdate()
        }

        IconButton {
            id: clockBtn
            source: "qrc:/images/clock.svg"
            anchors.top: parent.top
            anchors.left: parkbrakeBtn.right
            anchors.leftMargin: 30
            btnText: "12:20"
            function timeUpdate() {
                var date = new Date();
                var hour = date.getHours();
                var minute = date.getMinutes();
                if (hour < 10)
                    hour = "0" + hour;
                if (minute < 10)
                    minute = "0" + minute;
                btnText = hour + ":" + minute;
            }
            Component.onCompleted: timeUpdate();
        }
    }

    PressureStatusDlg {
        id: pressureDlg
    }

    WeightStatusDlg {
        id: weightDlg
    }

    ParkbrakeStatusDlg {
        id: parkbrakeDlg
    }
}
