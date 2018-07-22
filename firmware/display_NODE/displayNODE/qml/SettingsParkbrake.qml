import QtQuick 2.7
import QtQuick.Controls 2.2

import mummesoft 0.1
import "helpers.js" as Helpers

Item {
    id: root

    onVisibleChanged: {
        Helpers.switchToSettingsHeader(root);
    }

    Label {
        id: pageHeadline
        text: qsTr("Parkbrake settings")+ tr.str
        color: "white"
        font.pointSize: 20
        anchors.left: root.left
        anchors.leftMargin: 10
        anchors.top: root.top
        anchors.topMargin: 10
    }

    PageIndicator {
        id: pageIndicator
        anchors.top: root.top
        anchors.topMargin: 10
        anchors.horizontalCenter: root.horizontalCenter
        currentIndex: pageView.currentIndex
        interactive: true
        count: pageView.count
    }

    SwipeView {
        id: pageView
        anchors.top: pageHeadline.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        currentIndex: 0

        Item {
            id: absSettingsPage

            GroupBox {
                id: absSettings
                anchors.top: absSettingsPage.top
                anchors.topMargin: 10
                anchors.left: absSettingsPage.left
                anchors.leftMargin: 10
                anchors.right: absSettingsPage.right
                anchors.rightMargin: 10

                title: qsTr("ABS settings") + tr.str
                Grid {
                    columns: 3
                    //anchors.fill:absSettings
                    anchors.horizontalCenter: absSettings.horizontalCenter
                    spacing: 20
                    Column {
                        Label {
                            text: qsTr("Pulses per rev.") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: pulsesPerRevolution
                            settingsIndex: ParkbrakeConfig.S_PulsesPerRevolution //20
                            from: 20
                            to: 120
                            node: parkbrakeNode
                            warnDeadNodeInitialNode: true;
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Rim size") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: rimDiameter
                            settingsIndex: ParkbrakeConfig.S_RimDiameter // 21
                            from: 13
                            to: 20
                            node: parkbrakeNode
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Thread width") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: tireThread
                            settingsIndex: ParkbrakeConfig.S_TireThread // 22
                            from: 150
                            to: 320
                            stepSize: 5
                            node: parkbrakeNode
                        }
                    } // new row
                    Column {
                        Label {
                            text: qsTr("Tire profile") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: tireProfile
                            settingsIndex: ParkbrakeConfig.S_TireProfile // 23
                            from: 35
                            to: 80
                            stepSize: 5
                            node: parkbrakeNode
                        }
                    }
                    Column {
                        SettingsCheckBox {
                            id: parkbrakeUseAbs
                            settingsIndex: ParkbrakeConfig.S_AntilockParkbrake // 11
                            text: qsTr("ABS parkbrake when moving") + tr.str
                            node: parkbrakeNode
                        }
                    }
                } // end grid

            }
        }

        Item {
            id: motorSettingsPage
            GroupBox {
                id: motorCurrents
                title: qsTr("Motor currents") + tr.str;
                anchors.top: motorSettingsPage.top
                anchors.topMargin: 10
                anchors.left: motorSettingsPage.left
                anchors.leftMargin: 10
                anchors.right: motorSettingsPage.right
                anchors.rightMargin: 10

                Grid {
                    anchors.horizontalCenter: motorCurrents.horizontalCenter
                    columns: 4
                    Column {
                        Label {
                            text: qsTr("Current limit Releasing") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: currentMaxRelease
                            settingsIndex: ParkbrakeConfig.S_CurrentMaxRelease // 3
                            node: parkbrakeNode
                            from: 0
                            to: 30
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Current limit Tightening") + tr.str
                            color: "white"

                        }
                        SettingsSpinBox {
                            id: currentMaxTighten
                            settingsIndex: ParkbrakeConfig.S_CurrentMaxTighten // 9
                            node: parkbrakeNode
                            from: 0
                            to: 30
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Current freed threshhold") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: currentFreeThreshold
                            settingsIndex: ParkbrakeConfig.S_CurrentFreeThreshold // 2
                            node: parkbrakeNode
                            from: 0
                            to: 30
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Current tightened threshhold") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: currentTightenedThreshold
                            settingsIndex: ParkbrakeConfig.S_CurrentTightenedThreshold // 7
                            node: parkbrakeNode
                            from: 0
                            to: 30
                        }
                    }
                }
            }

            GroupBox {
                id: motorTimings
                title: qsTr("Motor timings (milliseconds)") + tr.str;
                anchors.top: motorCurrents.bottom
                anchors.topMargin: 10
                anchors.left: motorSettingsPage.left
                anchors.leftMargin: 10
                anchors.right: motorSettingsPage.right
                anchors.rightMargin: 10
                Grid {
                    anchors.fill: parent
                    columns: 4
                    Column {
                        Label {
                            text: qsTr("Startup time release") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: timeRevUpRelease
                            settingsIndex: ParkbrakeConfig.S_TimeRevupRelease // 0
                            node: parkbrakeNode
                            from: 0
                            to: 300
                        }

                        Label {
                            text: qsTr("Startup current release") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: currentStartupRelease
                            settingsIndex: ParkbrakeConfig.S_CurrentRevupMaxRelease // 4
                            node: parkbrakeNode
                            from: 0
                            to: 30
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Startup time tighten") + tr.str
                            color: "white"

                        }
                        SettingsSpinBox {
                            id: timeRevUpTighten
                            settingsIndex: ParkbrakeConfig.S_TimeRevupTighten // 5
                            node: parkbrakeNode
                            from: 0
                            to: 300
                        }

                        Label {
                            text: qsTr("Startup current tighten") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: currentStartupTighten
                            settingsIndex: ParkbrakeConfig.S_CurrentRevupMaxTighten // 8
                            node: parkbrakeNode
                            from: 0
                            to: 30
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Time after freed") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: timeAfterFreed
                            settingsIndex: ParkbrakeConfig.S_TimeContinueAfterFreed // 1
                            node: parkbrakeNode
                            from: 0
                            to: 1000
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Time after tightened") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: timeAfterTightened
                            settingsIndex: ParkbrakeConfig.S_TimeContinueAfterTightened // 6
                            node: parkbrakeNode
                            from: 0
                            to: 300
                        }
                    }
                }
            }
        } // end motor page
    }
}
