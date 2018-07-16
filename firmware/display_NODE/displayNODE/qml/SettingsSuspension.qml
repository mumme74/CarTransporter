import QtQuick 2.7
import QtQuick.Controls 2.2

import mummesoft 0.1
import "helpers.js" as Helpers
import "settings.js" as Settings

Item {
    id: root

    onVisibleChanged: {
        Helpers.switchToSettingsHeader(root);
    }

    Label {
        id: pageHeadline
        text: qsTr("Suspension settings")+ tr.str
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
            id: heightSettingsPage
            GroupBox {
                id: heightSettings
                anchors.top: heightSettingsPage.top
                anchors.topMargin: 10
                anchors.left: heightSettingsPage.left
                anchors.leftMargin: 10
                anchors.right: heightSettingsPage.right
                anchors.rightMargin: 10

                title: qsTr("Height settings") + tr.str
                Grid {
                    columns: 3
                    //anchors.fill:heightSettings
                    anchors.horizontalCenter: heightSettings.horizontalCenter
                    spacing: 20
                    Column {
                        Label {
                            text: qsTr("Left lower setpoint") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: leftLowSetpoint
                            settingsIndex: SuspensionConfig.LeftLowSetpoint_uint16
                            dataType: Settings.DataTypes.type_u16
                            node: suspensionNode
                            warnDeadNodeInitialNode: true // only warn on 1 control,
                                                          // otherwise we get a cascade of warnings
                            from: 0
                            to: 4095
                            stepSize: 10
                            value: 200
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Left normal setpoint") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: leftNormalSetpoint
                            settingsIndex: SuspensionConfig.LeftNormalSetpoint_uint16
                            dataType: Settings.DataTypes.type_u16
                            node: suspensionNode
                            from: 0
                            to: 4095
                            stepSize: 10
                            value: 1500
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Left higher setpoint") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: leftHighSetpoint
                            settingsIndex: SuspensionConfig.LeftHighSetpoint_uint16
                            dataType: Settings.DataTypes.type_u16
                            node: suspensionNode
                            from: 0
                            to: 4095
                            value: 3500
                            stepSize: 10
                        }
                    } // new row
                    Column {
                        Label {
                            text: qsTr("Right lower setpoint") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: rightLowSetpoint
                            settingsIndex: SuspensionConfig.RightLowSetpoint_uint16
                            dataType: Settings.DataTypes.type_u16
                            node: suspensionNode
                            from: 0
                            to: 4095
                            stepSize: 10
                            value: 200
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Right normal setpoint") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: rightNormalSetpoint
                            settingsIndex: SuspensionConfig.RightNormalSetpoint_uint16
                            dataType: Settings.DataTypes.type_u16
                            node: suspensionNode
                            from: 0
                            to: 4095
                            stepSize: 10
                            value: 1500
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Right higher setpoint") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: rightHighSetpoint
                            settingsIndex: SuspensionConfig.RightHighSetpoint_uint16
                            dataType: Settings.DataTypes.type_u16
                            node: suspensionNode
                            from: 0
                            to: 4095
                            stepSize: 10
                            value: 3500
                        }
                    } // new row
                    Column {
                        Label {
                            text: qsTr("Allowed +/- offpoint") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: deadBand
                            settingsIndex: SuspensionConfig.DeadBand_uint16
                            dataType: Settings.DataTypes.type_u16
                            node: suspensionNode
                            from: 10
                            to: 1000
                            stepSize: 10
                            value: 500
                        }
                    }
                    Column {
                        SettingsCheckBox {
                            id: leftReversedCheckBox
                            settingsIndex: SuspensionConfig.LeftHeightSensorReversed
                            text: qsTr("Left sensor reversed") + tr.str
                            node: suspensionNode
                        }
                    }
                    Column {
                        SettingsCheckBox {
                            id: rightReversedCheckBox
                            settingsIndex: SuspensionConfig.RightHeightSensorReversed
                            text: qsTr("Right sensor reversed") + tr.str
                            node: suspensionNode
                        }
                    }
                } // end grid
            }
        }
        Item {
            id: pidLoopPage
            GroupBox {
                id: pidLoopSettings
                anchors.top: pidLoopPage.top
                anchors.topMargin: 10
                anchors.left: pidLoopPage.left
                anchors.leftMargin: 10
                anchors.right: pidLoopPage.right
                anchors.rightMargin: 10

                title: qsTr("PID loop settings") + tr.str
                Grid {
                    id: pidGrid
                    columns: 3
                    anchors.horizontalCenter: pidLoopSettings.horizontalCenter
                    spacing: 20
                    Column {
                        Label {
                            id: p_factorLbl
                            text: qsTr("Position factor height loop") + tr.str
                            color: "white"
                        }
                        SettingsFloatSpinBox {
                            id: p_factor
                            settingsIndex: SuspensionConfig.KP_factor_float
                            node: suspensionNode
                            realFrom: 0.1
                            realTo: 40.0
                            realValue: 10.0
                            realStepSize: 0.1
                            decimals: 1
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Integral factor height loop") + tr.str
                            color: "white"
                        }
                        SettingsFloatSpinBox {
                            id: i_factor
                            settingsIndex: SuspensionConfig.KI_factor_float
                            node: suspensionNode
                            realFrom: 0.1
                            realTo: 40.0
                            realValue: 2.0
                            realStepSize: 0.1
                            decimals: 1
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Derivative factor height loop") + tr.str
                            color: "white"
                        }
                        SettingsFloatSpinBox {
                            id: d_factor
                            settingsIndex: SuspensionConfig.KD_factor_float
                            node: suspensionNode
                            realFrom: 0.1
                            realTo: 40.0
                            realValue: 2.0
                            realStepSize: 0.1
                            decimals: 1
                        }
                    }
                } // grid

                Label {
                    id: pidDescription
                    anchors.top: pidGrid.bottom
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    text: qsTr("P-factor sets how how fast it reacts to errors in setpoint\n" +
                               "I-factor cancels continous errors (ie. continuos error due weight)\n" +
                               "D-factor damps overswing") + tr.str
                    color: "white"

                    Component.onCompleted: {
                        pidLoopSettings.height = p_factorLbl.height +
                                    p_factor.height + height + pidGrid.spacing * 3 + 10;
                    }

                }
            } // grpbox
        } // pidLoopPage
        Item {
            id: weightPage
            GroupBox {
                id: weightSettings
                anchors.top: weightPage.top
                anchors.topMargin: 10
                anchors.left: weightPage.left
                anchors.leftMargin: 10
                anchors.right: weightPage.right
                anchors.rightMargin: 10

                title: qsTr("Weight settings") + tr.str
                Grid {
                    id: weightGrid
                    columns: 3
                    anchors.horizontalCenter: weightSettings.horizontalCenter
                    spacing: 20
                    Column {
                        Label {
                            text: qsTr("Empty chassi weight (kg)") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: emptyWeight
                            settingsIndex: SuspensionConfig.DeadWeight_uint16
                            node: suspensionNode
                            dataType: Settings.DataTypes.type_u16
                            from: 40
                            to: 400
                            value: 100
                            stepSize: 5
                        }
                    }
                    Column {
                        Label {
                            text: qsTr("Airbellows diameter (mm)") + tr.str
                            color: "white"
                        }
                        SettingsSpinBox {
                            id: cylDiameter
                            settingsIndex: SuspensionConfig.CylDia_mm
                            node: suspensionNode
                            dataType: Settings.DataTypes.type_u16
                            from: 40
                            to: 400
                            value: 100
                            stepSize: 1
                        }
                    }
                } // grid
            } // grpbox
        }
    }
}
