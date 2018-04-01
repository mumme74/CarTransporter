#include "cansuspensionnode.h"
#include "can_protocol.h"
#include "suspension_config.h"
#include "PID.h"
#include <QDebug>


CanSuspensionNode::CanSuspensionNode(CanInterface *canInterface, QObject *parent) :
    CanAbstractNode(C_suspensionNode, canInterface, parent)
{
    // init some known PIDS
    QCanBusFrame pid1;
    pid1.setExtendedFrameFormat(false);
    pid1.setFrameId(CAN_MSG_TYPE_UPDATE | C_suspensionUpdPID_1 | C_suspensionNode);
    pid1.setPayload(QByteArray("\0\0\0\0\0\0\0\0", 8));

    QCanBusFrame pid2;
    pid2.setExtendedFrameFormat(false);
    pid2.setFrameId(CAN_MSG_TYPE_UPDATE | C_suspensionUpdPID_2 | C_suspensionNode);
    pid2.setPayload(QByteArray("\0\0\0\0\0\0\0\0", 8));

    QCanBusFrame pid3;
    pid3.setExtendedFrameFormat(false);
    pid3.setFrameId(CAN_MSG_TYPE_UPDATE | C_suspensionUpdPID_3 | C_suspensionNode);
    pid3.setPayload(QByteArray("\0\0\0\0\0\0\0\0", 8));

    QCanBusFrame pid4;
    pid4.setExtendedFrameFormat(false);
    pid4.setFrameId(CAN_MSG_TYPE_UPDATE | C_suspensionUpdPID_4 | C_suspensionNode);
    pid4.setPayload(QByteArray("\0\0\0\0\0\0", 6));

    QList<QCanBusFrame> frames;
    frames << pid1 << pid2 << pid3 << pid4;

    updatedFromCan(frames);

    qmlRegisterUncreatableType<CanSuspensionNode>("mummesoft", 0, 1, "suspensionNode", QStringLiteral("Cant create CanSuspensionNode from QML"));

}

CanSuspensionNode::~CanSuspensionNode()
{
}


bool CanSuspensionNode::fetchDtc(int storedIdx, QJSValue jsCallback)
{
    return CanAbstractNode::fetchDtc(storedIdx, jsCallback, C_suspensionDiagGetDTC);
}

void CanSuspensionNode::fetchAllDtcs()
{
    CanAbstractNode::fetchAllDtcs(C_suspensionDiagDTCLength);
}

void CanSuspensionNode::clearAllDtcs()
{
    CanAbstractNode::clearAllDtcs(C_suspensionDiagClearDTC);
}

bool CanSuspensionNode::activateOutput(quint8 pid, quint8 vlu) const
{
    if (pid == 0 || pid > PIDs::compressorPWM_8bit)
        return false;

    QByteArray pl(pid, vlu);
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_suspensionDiagActuatorTest | C_displayNode, pl);
    f.setExtendedFrameFormat(false);
    m_canIface->sendFrame(f);
    return true;
}

bool CanSuspensionNode::clearActivateOutput(quint8 pid) const
{
    if (pid == 0 || pid > PIDs::compressorPWM_8bit)
        return false;

    QByteArray pl;
    pl.append(pid);
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_suspensionDiagClearActuatorTest | C_displayNode, pl);
    f.setExtendedFrameFormat(false);
    m_canIface->sendFrame(f);
    return true;
}

bool CanSuspensionNode::fetchSetting(quint8 idx, QJSValue jsCallback)
{
    if (idx >= Configs::ConfigEnd)
        return false;

    return CanAbstractNode::fetchSetting(idx, jsCallback, C_suspensionCmdGetConfig);
}

bool CanSuspensionNode::setSettingUint16(quint8 idx, quint16 vlu, QJSValue jsCallback)
{
    if (idx >= Configs::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingU16(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

bool CanSuspensionNode::setSettingUint32(quint8 idx, quint32 vlu, QJSValue jsCallback)
{
    if (idx >= Configs::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingU32(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

bool CanSuspensionNode::setSettingFloat(quint8 idx, float vlu, QJSValue jsCallback)
{
    if (idx >= Configs::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingF(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

void CanSuspensionNode::updateCanFrame(const QCanBusFrame &frame)
{
    quint32 sid = frame.frameId();
    QByteArray data = frame.payload();
    quint16 msgId = sid & CAN_MSG_ID_MASK;

    switch (msgId) {
    case C_suspensionUpdPID_1: {
        // response:
        //    [0:7]          [0:7]         [0:7]          [0:7]            [0:7]           [0:7]           [0:7]           [0:7]
        // leftFillDuty | leftDumpDuty | leftSuckDuty | rightFillDuty | rightDumpDuty | rightSuckDuty | airDryerDuty | compressorDuty
        setPidsValue(tr("LeftFill_duty"), QString::number(data[0]), "%", m_pids, C_suspensionNode);
        setPidsValue(tr("LeftDump_duty"), QString::number(data[1]), "%", m_pids, C_suspensionNode);
        setPidsValue(tr("LeftSuck_duty"), QString::number(data[2]), "%", m_pids, C_suspensionNode);
        setPidsValue(tr("RightFill_duty"), QString::number(data[3]), "%", m_pids, C_suspensionNode);
        setPidsValue(tr("RightDump_duty"), QString::number(data[4]), "%", m_pids, C_suspensionNode);
        setPidsValue(tr("RightSuck_duty"), QString::number(data[5]), "%", m_pids, C_suspensionNode);
        setPidsValue(tr("Airdryer_duty"), QString::number(data[6]), "%", m_pids, C_suspensionNode);
        setPidsValue(tr("Compressor_duty"), QString::number(data[7]), "%", m_pids, C_suspensionNode);
    } break;
    case C_suspensionUpdPID_2: {
        // response:
        //   [0:15]              [0:15]        [0:15]          [0:7]             [0:7]
        // systemPressure | leftPressure | rightPressure | compressorCurrent | spare1Duty
        //    12bit              12bit          12bit
        setPidsValue(tr("System_pressure"), QString::number(data[1] << 8 | data[0]), "bar", m_pids, C_suspensionNode);
        setPidsValue(tr("Left_pressure"), QString::number(data[3] << 8 | data[2]), "bar", m_pids, C_suspensionNode);
        setPidsValue(tr("Right_pressure"), QString::number(data[5] << 8 | data[4]), "bar", m_pids, C_suspensionNode);
        setPidsValue(tr("Compressor_current"), QString::number(data[6]), "A", m_pids, C_suspensionNode);
        setPidsValue(tr("Spare1_duty"), QString::number(data[7]), "%", m_pids, C_suspensionNode);
    } break;
    case C_suspensionUpdPID_3: {
        // response:
        //   [0:15]      [0:15]            [0:15]       [0:15]
        // leftHeight | rightHeight | compressorTemp | spareAnalog1
        //  12bit          12bit           12bit        12bit
        setPidsValue(tr("Left_height"), QString::number(data[1] << 8 | data[0]), "steps", m_pids, C_suspensionNode);
        setPidsValue(tr("Right_height"), QString::number(data[3] << 8 | data[2]), "steps", m_pids, C_suspensionNode);
        setPidsValue(tr("Compressor_temp"), QString::number(data[5] << 8 | data[4]), "steps", m_pids, C_suspensionNode);
        setPidsValue(tr("SpareAnalog1_steps"), QString::number(data[7] << 8 | data[6]), "steps", m_pids, C_suspensionNode);

    } break;
    case C_suspensionUpdPID_4: {
        // response:
        //     [0:15]          [0:15]        [0:15]
        //  airFeedState    heightState     loadedWeight
        //   Pid::States     Pid::States      in kg
        setAirFeedStatePid(tr("AirFeed_state"), data[1] << 8 | data[0], m_pids);
        setHeightStatePid(tr("Height_state"), data[3] << 8 | data[2], m_pids);
        setPidsValue(tr("Load_weight"), QString::number(data[5] << 8 | data[4]), "kg", m_pids, C_suspensionNode);

    } break;
    default: {
        // unknown
        QString key = QString::number(sid, 16);
        QString vlu;
        for (quint8 v : data) {
            vlu += QString::number(v, 16);
        }
        setPidsValue(key, vlu, "", m_pids, C_suspensionNode);
        break;
    }
    }

}

void CanSuspensionNode::commandCanFrame(const QCanBusFrame &frame)
{
    // when parkbrakenode sends a command frame
    QByteArray payload = frame.payload();
    quint32 sid = frame.frameId();
    switch (sid & CAN_MSG_ID_MASK) {
    case C_suspensionCmdGetConfig: {
        quint8 idx = payload[0];
        if (frame.payload().size() == 3) {
            quint16 vlu = (payload[2] << 8) | payload[1];
            settingsFetchArrival(idx, vlu);
        } else if (payload.size() == 5) {
            // FIXME need a better algorithm here, 32bit uint also has 4 bytes
            quint32 u_vlu = (payload[4] << 24) | (payload[3] << 16) | (payload[2] << 8) | (payload[1]);
            float vlu = (float)u_vlu;
            settingsFetchArrivalFloat(idx, vlu);
        }
    } break;
    case C_suspensionCmdSetConfig: {
        quint8 idx = payload[0];
        quint8 res = payload[1];
        settingsSetArrival(idx, res);
    } break;
    default:
        break;
    }
}

void CanSuspensionNode::exceptionCanFrame(const QCanBusFrame &frame)
{
    // when parkbrakeNode sends a exceptionFrame
    QByteArray payload = frame.payload();
    quint32 sid = frame.frameId();
    switch (sid & CAN_MSG_ID_MASK) {
    case C_suspensionExcDTC: {
        // a error code has been set in suspension
        //  [0:7]        [0:15]    [0:6] [:7]
        // stored nr     dtc code  occurrences & pending mask (1 on 7th bit = real code, 0=pending)
        quint8 occurences = payload[3] & 0x8F;
        bool pending = static_cast<quint8>(payload[3]) < 0x8F;
        quint16 code = (payload[2] << 8) | payload[1];
        CanDtc *dtc = getDtc(payload[0]);
        if (dtc == nullptr) {
            dtc = new CanDtc(this, payload[0], code, occurences, 0);
            dtc->setPending(pending);
            m_dtcs.insert(dtc->storedNr(), dtc);
            emit dtcArrived(dtc->storedNr());
        } else {
            dtc->setPending(pending);
            dtc->setOccurences(occurences);
        }
        emit newDtcSet(dtc);
    }    break;
    case C_suspensionExcUserError: {
        // [0:15]
        // userError id in enum
        quint16 excId = (payload.at(1) << 8) | payload.at(0);
        if (excId == C_userErrorHeightNonValidState) {
            emit userErrorHeightNonValidState();
        } else if (excId == C_userErrorSuckedRearWheelBlocked) {
            emit userErrorSuckingNotAllowed();
        }

        emit userError(excId);
    } break;
    default:
        break;
    }
}

void CanSuspensionNode::diagCanFrame(const QCanBusFrame &frame)
{
    // when uspensionNode sends a diagnose frame
    QByteArray payload = frame.payload();
    quint32 sid = frame.frameId();
    switch (sid & CAN_MSG_ID_MASK) {
    case C_suspensionDiagDTCLength:
        // response
        //  [0:7] the length(number) of stored dtc's
        if (payload.size()) {
            m_dtcCount = payload[0];
            emit dtcCountChanged(m_dtcCount);

            if (m_dtcs.size() != m_dtcCount) {
                // refetch automatically
                m_dtcs.clear();
                // fetch all others
                for (quint8 i = 0; i < m_dtcCount; ++i) {
                    QByteArray pl(i, 1);
                    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_suspensionDiagGetDTC | C_displayNode, pl);
                    f.setExtendedFrameFormat(false);
                    m_canIface->sendFrame(f);
                }
            }
        }
        break;

    case C_suspensionDiagClearDTC:
        // response  [0:7] number erased
        if (payload.size()) {
            if (m_dtcCount != payload[0]) {
                qDebug() << "dtcCount mismatch on erase, stored:"
                         << m_dtcCount << " cleared:" << payload[0] << endl;
            }
            m_dtcCount = 0;
            m_dtcs.clear();
            m_freezeFrames.clear();
            bool cleared = (quint8)(payload[0]) != 0;
            if (!cleared)
                cleared = m_dtcCount == 0; // when clearing a empty dtcs list
            emit dtcsCleared(cleared);
            emit dtcCountChanged(m_dtcCount);

        }

        break;

    case C_suspensionDiagGetDTC: {
        // response:
        //  [0:7]        [0:15]    [0:6] [:7]
        // stored nr     dtc code  occurrences & pending mask
        int storedNr = payload[0];
        quint16 code = (payload[2] << 8) | payload[1];
        quint8 occurences = payload[3];
        if (m_dtcs.contains(storedNr)) {
            CanDtc *dtc = m_dtcs[storedNr];
            dtc->setOccurences(occurences & 0x7F);
            dtc->setPending(occurences & 0x80); // bit7 is pending marker
        } else {
            CanDtc *dtc = new CanDtc(this, storedNr, code, occurences & 0x7F, 0);
            m_dtcs.insert(storedNr, dtc);
        }
        dtcOnArrival(storedNr);
        emit dtcArrived(storedNr);

    }   break;

    case C_suspensionDiagActuatorTest: {
        // response:
        //  [0:7]         [0:7]
        // pid adress     value
        emit activateOutputComfirmed(payload[0], payload[1]);

        quint16 id = payload[0];
        switch (static_cast<PIDs::IDs>(id)) {
        case PIDs::leftFillPWM_8bit: {
            setPidsValue(tr("LeftFill_duty"), QString::number(payload[1]), "%", m_pids, C_suspensionNode);
        } break;
        case PIDs::leftDumpPWM_8bit: {
            setPidsValue(tr("LeftDump_duty"), QString::number(payload[1]), "%", m_pids, C_suspensionNode);
        } break;
        case PIDs::leftSuckPWM_8bit: {
            setPidsValue(tr("LeftSuck_duty"), QString::number(payload[1]), "%", m_pids, C_suspensionNode);
        } break;
        case PIDs::rightFillPWM_8bit: {
            setPidsValue(tr("RightFill_duty"), QString::number(payload[1]), "%", m_pids, C_suspensionNode);
        } break;
        case PIDs::rightDumpPWM_8bit: {
            setPidsValue(tr("RightDump_duty"), QString::number(payload[1]), "%", m_pids, C_suspensionNode);
        } break;
        case PIDs::rightSuckPWM_8bit: {
            setPidsValue(tr("RightSuck_duty"), QString::number(payload[1]), "%", m_pids, C_suspensionNode);
        } break;
        case PIDs::airdryerPWM_8bit: {
            setPidsValue(tr("Airdryer_duty"), QString::number(payload[1]), "%", m_pids, C_suspensionNode);
        } break;
        case PIDs::suspensionSpare1PWM_8bit: {
            setPidsValue(tr("Spare1_duty"), QString::number(payload[1]), "%", m_pids, C_suspensionNode);
        } break;
        case PIDs::compressorPWM_8bit: {
            setPidsValue(tr("Compressor_duty"), QString::number(payload[1]), "%", m_pids, C_suspensionNode);
        } break;
        default:
            break;
        }

    }   break;
    case C_suspensionDiagClearActuatorTest: {
        // response:
        //  [0:7]
        emit clearActivateOutput(payload[0]);

        // request a update pid from node
        can_msgIdsUpdate_e canUpdId = C_suspensionUpdPID_1;

        if (static_cast<quint8>(payload[0]) == PIDs::suspensionSpare1PWM_8bit)
            canUpdId = C_suspensionUpdPID_2;

        QCanBusFrame f(QCanBusFrame::RemoteRequestFrame);
        f.setFrameId(CAN_MSG_TYPE_DIAG | canUpdId | C_displayNode);
        f.setExtendedFrameFormat(false);
        m_canIface->sendFrame(f);

    } break;
    default:
        break;
    }
}

void CanSuspensionNode::setAirFeedStatePid(QString key, quint16 state, PidStore &pidStore)
{
    QString value;
    switch (static_cast<PID::States>(state)) {
    case PID::Off:
        value = tr("Off");
        break;
    case PID::On:
        value = tr("On");
        break;
    case PID::RevUp:
        value = tr("SpinUp");
        break;
    case PID::FullRev:
        value = tr("FullRev");
        break;
    case PID::RevDown:
        value = tr("SlowDown");
        break;
    case PID::Airdryer:
        value = tr("Airdryer");
        break;
    case PID::Error:
        value = tr("ErrorState");
        break;
    default:
        value = tr("UnknownState");
        break;
    }

    setPidsValue(key, value, "", pidStore, C_suspensionNode);
}

void CanSuspensionNode::setHeightStatePid(QString key, quint16 state, CanAbstractNode::PidStore &pidStore)
{
    QString value;
    switch (static_cast<PID::States>(state)) {
    case PID::LowState:
        value = tr("At_low");
        break;
    case PID::ToLowState:
        value = tr("Lowering");
        break;
    case PID::NormalState:
        value = tr("At_middle");
        break;
    case PID::ToNormalState:
        value = tr("To_middle");
        break;
    case PID::HighState:
        value = tr("At_Max");
        break;
    case PID::ToHighState:
        value = tr("To_Max");
        break;
    case PID::Error:
        value = tr("ErrorState");
        break;
    default:
        value = tr("UnknownState");
        break;
    }

    setPidsValue(key, value, "", pidStore, C_suspensionNode);
}



