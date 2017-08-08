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
        text: qsTr("Parkbrake settings")+ tr.str
        color: "white"
        font.pointSize: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    PageIndicator {
        id: pageIndicator
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        currentIndex: pageView.currentIndex
        interactive: true
        count: pageView.count
    }

    SwipeView {
        id: pageView
        anchors.top: pageHeadline.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: 0

        Item {
            id: absSettingsPage

            GroupBox {
                id: absSettings
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10

                title: qsTr("ABS settings") + tr.str
                Grid {
                    columns: 4
                    anchors.fill:absSettings
                    Column {
                        Label {
                            text: qsTr("Pulses per rev.") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: pulsesPerRevolution
                            settingsIndex: 20
                            from: 20
                            to: 120
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Rim size") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: rimDiameter
                            settingsIndex: 21
                            from: 13
                            to: 20
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Thread width") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: tireThread
                            settingsIndex: 22
                            from: 150
                            to: 320
                            stepSize: 5
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Tire profile") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: tireProfile
                            settingsIndex: 23
                            from: 35
                            to: 80
                            stepSize: 5
                        }
                    }
                    // new row
                    Column {
                        SettingsCheckBox {
                            id: parkbrakeUseAbs
                            settingsIndex: 11
                            text: qsTr("ABS parkbrake when moving") + tr.str

                        }
                    }
                } // end grid

            }
        }

        Item {
            id: motorSettings
            GroupBox {
                id: motorCurrents
                title: qsTr("Motor currents") + tr.str;
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10

                Grid {
                    anchors.fill: parent
                    columns: 4
                    Column {
                        Label {
                            text: qsTr("Current limit Releasing") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: currentMaxRelease
                            settingsIndex: 3
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
                            settingsIndex: 9
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
                            settingsIndex: 2
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
                            settingsIndex: 7
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
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
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
                            settingsIndex: 0
                            from: 0
                            to: 300
                        }

                        Label {
                            text: qsTr("Startup current release") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: currentStartupRelease
                            settingsIndex: 4
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
                            settingsIndex: 5
                            from: 0
                            to: 300
                        }

                        Label {
                            text: qsTr("Startup current tighten") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: currentStartupTighten
                            settingsIndex: 8
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
                            settingsIndex: 1
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
                            settingsIndex: 6
                            from: 0
                            to: 300
                        }
                    }
                }
            }
        } // end motor page

    }
}
