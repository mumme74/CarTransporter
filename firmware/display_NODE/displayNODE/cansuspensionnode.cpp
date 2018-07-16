#include "cansuspensionnode.h"
#include "can_protocol.h"
#include "PID.h"
#include <QDebug>

// #define TEST_DIAG 1


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

#ifdef TEST_DIAG
    // ------------- test ------------------------
    // for test
    QCanBusFrame dtcLength;
    dtcLength.setExtendedFrameFormat(false);
    dtcLength.setFrameId(CAN_MSG_TYPE_DIAG | C_suspensionDiagDTCLength | C_suspensionNode);
    QByteArray pl;
    pl.insert(0, 1);
    dtcLength.setPayload(pl);

    QCanBusFrame dtc1;
    dtc1.setExtendedFrameFormat(false);
    dtc1.setFrameId(CAN_MSG_TYPE_EXCEPTION | C_suspensionExcDTC | C_suspensionNode);
    pl.clear();
    pl.insert(0, '\0');
    pl.insert(1, 0x81);
    pl.insert(2, 0x11);
    pl.insert(3, 0x87);
    pl.insert(4, 0x3A);
    pl.insert(5, 0x42);
    dtc1.setPayload(pl);

    frames << dtcLength << dtc1;

    // ------------ end test -------------------
#endif

    updatedFromCan(frames);

    qmlRegisterUncreatableType<CanSuspensionNode>("mummesoft", 0, 1, "suspensionNode", QStringLiteral("Cant create CanSuspensionNode from QML"));
    qmlRegisterUncreatableType<SuspensionConfig>("mummesoft", 0, 1, "SuspensionConfig",
                                                 "Cannot create SuspensionConfig in QML");
}

CanSuspensionNode::~CanSuspensionNode()
{
}


bool CanSuspensionNode::fetchDtc(int storedIdx, QJSValue jsCallback)
{
    return CanAbstractNode::fetchDtc(storedIdx, jsCallback, C_suspensionDiagGetDTC);
}

bool CanSuspensionNode::fetchAllDtcs()
{
    return CanAbstractNode::fetchAllDtcs(C_suspensionDiagDTCLength);
}

bool CanSuspensionNode::clearAllDtcs()
{
    return CanAbstractNode::clearAllDtcs(C_suspensionDiagClearDTC);
}

bool CanSuspensionNode::activateOutput(quint8 pid, quint8 vlu) const
{
    if (pid == 0 || pid > PIDs::compressorPWM_8bit)
        return false;

    QByteArray pl;
    pl.append(pid);
    pl.append(vlu);
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_suspensionDiagActuatorTest | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

bool CanSuspensionNode::clearActivateOutput(quint8 pid) const
{
    if (pid == 0 || pid > PIDs::compressorPWM_8bit)
        return false;

    QByteArray pl;
    pl.append(pid);
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_suspensionDiagClearActuatorTest | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

bool CanSuspensionNode::fetchSetting(quint8 idx, QJSValue jsCallback)
{
    if (idx >= SuspensionConfig::ConfigEnd)
        return false;

    return CanAbstractNode::fetchSetting(idx, jsCallback, C_suspensionCmdGetConfig);
}

bool CanSuspensionNode::setSettingUInt8(quint8 idx, quint8 vlu, QJSValue jsCallback)
{
    if (idx >= SuspensionConfig::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingU8(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

bool CanSuspensionNode::setSettingUInt16(quint8 idx, quint16 vlu, QJSValue jsCallback)
{
    if (idx >= SuspensionConfig::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingU16(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

bool CanSuspensionNode::setSettingUInt32(quint8 idx, quint32 vlu, QJSValue jsCallback)
{
    if (idx >= SuspensionConfig::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingU32(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

bool CanSuspensionNode::setSettingFloat(quint8 idx, float vlu, QJSValue jsCallback)
{
    if (idx >= SuspensionConfig::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingF(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

bool CanSuspensionNode::setHeightState(const QString &state, QJSValue jsCallback)
{
    if (!jsCallback.isCallable())
        return false;

    can_msgIdsCommand_e canCmdId;

    if (state == QStringLiteral("ToLowState")) {
        canCmdId = C_suspensionCmdSetLow;
    } else if (state == QStringLiteral("ToNormalState")) {
        canCmdId = C_suspensionCmdSetNormal;
    } else if (state == QStringLiteral("ToHighState")) {
        canCmdId = C_suspensionCmdSetHigh;
    } else {
        return false;
    }

    m_heightStateSetCallback.insert(canCmdId, jsCallback);

    // send to Canbus
    QCanBusFrame f;//(QCanBusFrame::RemoteRequestFrame);// FlexCAN hardware limitation, can't recieve rtr
    f.setFrameId(CAN_MSG_TYPE_COMMAND | canCmdId | C_displayNode);
    return m_canIface->sendFrame(f);
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
        setPidsValue(tr("System_pressure"), QString::number(data[1] << 8 | (0x00ff & data[0])), "kPa", m_pids, C_suspensionNode);
        setPidsValue(tr("Left_pressure"), QString::number(data[3] << 8 | (0x00ff & data[2])), "kPa", m_pids, C_suspensionNode);
        setPidsValue(tr("Right_pressure"), QString::number(data[5] << 8 | (0x00ff & data[4])), "kPa", m_pids, C_suspensionNode);
        setPidsValue(tr("Compressor_current"), QString::number(data[6]), "A", m_pids, C_suspensionNode);
        setPidsValue(tr("Spare1_duty"), QString::number(data[7]), "%", m_pids, C_suspensionNode);
    } break;
    case C_suspensionUpdPID_3: {
        // response:
        //   [0:15]      [0:15]            [0:15]       [0:15]
        // leftHeight | rightHeight | compressorTemp | spareAnalog1
        //  12bit          12bit           12bit        12bit
        setPidsValue(tr("Left_height"), QString::number(data[1] << 8 | (0x00ff & data[0])), "steps", m_pids, C_suspensionNode);
        setPidsValue(tr("Right_height"), QString::number(data[3] << 8 | (0x00ff & data[2])), "steps", m_pids, C_suspensionNode);
        setPidsValue(tr("Compressor_temp"), QString::number((qint16)(data[5] << 8 | (0x00ff & data[4]))), "°C", m_pids, C_suspensionNode);
        setPidsValue(tr("SpareAnalog1_steps"), QString::number(data[7] << 8 | (0x00ff & data[6])), "steps", m_pids, C_suspensionNode);

    } break;
    case C_suspensionUpdPID_4: {

        // response:
        //     [0:15]          [0:15]        [0:15]
        //  airFeedState    heightState     loadedWeight
        //   Pid::States     Pid::States      in kg
        setAirFeedStatePid(tr("AirFeed_state"), data[1] << 8 | (0x00ff & data[0]), m_pids);
        setHeightStatePid(tr("Height_state"), data[3] << 8 | (0x00ff & data[2]), m_pids);
        setPidsValue(tr("Load_weight"), QString::number(data[5] << 8 | (0x00ff & data[4])), "kg", m_pids, C_suspensionNode);
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
    // when suspensionnode sends a command frame
    QByteArray payload = frame.payload();
    quint32 sid = frame.frameId();
    switch (sid & CAN_MSG_ID_MASK) {
    case C_suspensionCmdGetConfig: {
        quint8 idx = payload[0];
        if (frame.payload().size() == 3) {
            quint16 vlu = (payload[2] << 8) | (0x00ff & payload[1]);
            settingsFetchArrival(idx, vlu);
        } else if (payload.size() == 5) {
            quint32 u_vlu = 0;
            u_vlu |= payload[1] & 0x000000FF;
            u_vlu |= (payload[2] << 8) & 0x0000FF00;
            u_vlu |= (payload[3] << 16) & 0x00FF0000;
            u_vlu |= (payload[4] << 24) & 0xFF000000;

            // is it a float?
            if (idx >= SuspensionConfig::KP_factor_float &&
                idx <= SuspensionConfig::KD_factor_float)
            {
                float vlu = *(float*)&u_vlu;
                settingsFetchArrivalFloat(idx, vlu);
            } else {
                settingsFetchArrival32(idx, u_vlu);
            }
        }
    } break;
    case C_suspensionCmdSetConfig: {
        quint8 idx = payload[0];
        quint8 res = payload[1];
        settingsSetArrival(idx, res);
    } break;
    case C_suspensionCmdSetLow: // fallthrough
    case C_suspensionCmdSetNormal:
    case C_suspensionCmdSetHigh:
    case C_suspensionCmdSetRearWheelsUp: {
        if (payload[0] != 0xAA)
            break;
        int idx = sid & CAN_MSG_ID_MASK;
        if (m_heightStateSetCallback.contains(idx)) {
            QJSValue jsCallback = m_heightStateSetCallback.take(idx);
            jsCallback.call(QJSValueList { static_cast<quint8>(payload[0]) });
        }
    }   break;
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
        quint8 occurences = payload[3] & 0x7F;
        bool pending = static_cast<quint8>(payload[3]) < 0x80;
        quint16 code = (payload[2] << 8) | (0x00ff & payload[1]);
        CanDtc *dtc = getDtc(payload[0]);
        if (dtc == nullptr) {
            dtc = new CanDtc(this, payload[0], code, occurences, 0);
            dtc->setPending(pending);
            m_dtcs.insert(dtc->storedNr(), dtc);
            emit dtcAdded(dtc->storedNr());
        } else {
            dtc->setPending(pending);
            dtc->setOccurences(occurences);
            emit dtcUpdated(dtc->storedNr());
        }
    }    break;
    case C_suspensionExcUserError: {
        // [0:15]
        // userError id in enum
        quint16 excId = (payload.at(1) << 8) | (0x00ff & payload.at(0));
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
                    QByteArray pl;
                    pl.append(i);
                    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_suspensionDiagGetDTC | C_displayNode, pl);
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
            emit dtcsCleared(cleared, (quint8)(payload[0]));
            emit dtcCountChanged(m_dtcCount);

        }

        break;

    case C_suspensionDiagGetDTC: {
        // response:
        //  [0:7]        [0:15]    [0:6] [:7]
        // stored nr     dtc code  occurrences & pending mask
        int storedNr = payload[0];
        quint16 code = (payload[2] << 8) | (0x00ff & payload[1]);
        quint8 occurences = payload[3];
        if (m_dtcs.contains(storedNr)) {
            CanDtc *dtc = m_dtcs[storedNr];
            dtc->setOccurences(occurences & 0x7F);
            dtc->setPending(occurences & 0x80); // bit7 is pending marker
            emit dtcUpdated(storedNr);
        } else {
            CanDtc *dtc = new CanDtc(this, storedNr, code, occurences & 0x7F, 0);
            m_dtcs.insert(storedNr, dtc);
            emit dtcAdded(storedNr);
        }
        dtcArriveDoQmlCallback(storedNr);

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
        emit clearActivateOutputComfirmed(payload[0]);

        // request a update pid from node
        can_msgIdsUpdate_e canUpdId = C_suspensionUpdPID_1;

        if (static_cast<quint8>(payload[0]) == PIDs::suspensionSpare1PWM_8bit)
            canUpdId = C_suspensionUpdPID_2;

        QCanBusFrame f; //(QCanBusFrame::RemoteRequestFrame); // FlexCAN hardware limitation, can't recieve rtr
        f.setFrameId(CAN_MSG_TYPE_UPDATE | canUpdId | C_displayNode);
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
    QString value, stateStr;
    switch (static_cast<PID::States>(state)) {
    case PID::LowState:
        value = tr("At_low");
        stateStr = "LowState";
        break;
    case PID::ToLowState:
        value = tr("Lowering");
        stateStr = "ToLowerState";
        break;
    case PID::NormalState:
        value = tr("At_middle");
        stateStr = "NormalState";
        break;
    case PID::ToNormalState:
        value = tr("To_middle");
        stateStr = "ToNormalState";
        break;
    case PID::HighState:
        value = tr("At_Max");
        stateStr = "HighState";
        break;
    case PID::ToHighState:
        value = tr("To_Max");
        stateStr = "ToHighState";
        break;
    case PID::Error:
        value = tr("ErrorState");
        stateStr = "ErrorState";
        break;
    default:
        value = tr("UnknownState");
        stateStr = "UnknownState";
        break;
    }

    // first find our old value on state
    bool stateChanged = false;
    if (m_pids.keys().contains(tr("Height_state")) &&
        m_pids[tr("Height_state")]->valueStr() != value)
        stateChanged = true;

    setPidsValue(key, value, "", pidStore, C_suspensionNode);

    // now that state has changed we can emit
    if (stateChanged)
        emit heightStateChanged(stateStr);
}


// -------------------------------------------------------------------


SuspensionPressureDlgModel::SuspensionPressureDlgModel(CanSuspensionNode *node,
                                                       QObject *parent) :
    QAbstractTableModel(parent),
    m_node(node)
{
    int maxRows = 0;
    do {
        QModelIndex idx = createIndex(maxRows, 0, nullptr);
        if (!data(idx, KeyRole).isValid())
            break;
        ++maxRows;
    } while (true);

    m_rowCount = maxRows;

    // connect for updates
    for (int i = 0; i < m_rowCount; ++i) {
        CanPid *pid = getPidForIdx(i);
        connect(pid, SIGNAL(valueChanged(const CanPid*)),
                this, SLOT(pidUpdated(const CanPid*)));
    }

    qmlRegisterUncreatableType<SuspensionPressureDlgModel>("mummesoft", 0, 1, "SuspensionPressureDlgModel", QStringLiteral("Cant create SuspensionPressureDlgModel from QML"));
}

SuspensionPressureDlgModel::~SuspensionPressureDlgModel()
{

}

QVariant SuspensionPressureDlgModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((role != Qt::DisplayRole && role !=Qt::ToolTipRole) || orientation != Qt::Horizontal)
        return QVariant();

    if (section < 0 || section >= colCount)
        return QVariant();

    if (section == 0)
        return QVariant(tr("Name"));
    else if (section == 1)
        return QVariant(tr("Value"));
    return QVariant();
}

QVariant SuspensionPressureDlgModel::data(const QModelIndex &index, int role) const
{
    if (index.column() < 0 || index.column() >= colCount)
        return QVariant();

    if (index.row() >= m_rowCount && m_rowCount > 0)
        return QVariant();

    CanPid *pid = getPidForIdx(index.row());

    if (!pid)
        return QVariant();

    if (role == KeyRole)
        return pid->key();
    else if (role == ValueRole)
        return QVariant(pid->valueStr() + pid->unit());
    else
        return QVariant();
}

int SuspensionPressureDlgModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return colCount;
}

int SuspensionPressureDlgModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowCount;
}

QHash<int, QByteArray> SuspensionPressureDlgModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "name";
    roles[ValueRole] = "value";
    return roles;
}

CanPid *SuspensionPressureDlgModel::getPidForIdx(int idx) const
{
    switch (idx) {
    case 0:
        return m_node->getPid(tr("System_pressure"));
    case 1:
        return m_node->getPid(tr("Compressor_duty"));
    case 2:
        return m_node->getPid(tr("Airdryer_duty"));
    case 3:
        return m_node->getPid(tr("Left_pressure"));
    case 4:
        return m_node->getPid(tr("Right_pressure"));
    default:
        return nullptr;
    }
}

void SuspensionPressureDlgModel::pidUpdated(const CanPid *pid)
{
    int i = 0;
    for (; i < m_rowCount; ++i) {
        if (getPidForIdx(i) == pid)
            break;
    }

    if (i >= m_rowCount)
        return;

    QModelIndex left = createIndex(i, 0, (void*)pid);
    QModelIndex right = createIndex(i, 1, (void*)pid);
    dataChanged(left, right);
}

// -----------------------------------------------------------

SuspensionDTCModel::SuspensionDTCModel(CanSuspensionNode *node) :
    QAbstractTableModel(node),
    m_node(node)
{
    connect(m_node, SIGNAL(dtcUpdated(int)), this, SLOT(updated(int)));
    connect(m_node, SIGNAL(dtcAdded(int)), this, SLOT(added(int)));
    connect(m_node, SIGNAL(dtcsCleared(bool, int)), this, SLOT(cleared(bool, int)));

    qmlRegisterUncreatableType<SuspensionDTCModel>("mummesoft", 0, 1, "SuspensionDTCModel", QStringLiteral("Cant create SuspensionDTCModel from QML"));
}

SuspensionDTCModel::~SuspensionDTCModel()
{
}

QVariant SuspensionDTCModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role);
    Q_UNUSED(orientation);
    return QVariant();

    if (section < 0 || section >= m_colCount)
        return QVariant();

    switch (section) {
    case 0:
        return QVariant(tr("Code"));
    case 1:
        return QVariant(tr("Description"));
    case 2:
        return QVariant(tr("Occurences"));
    case 3:
        return QVariant(tr("Pending"));
    default:
        return QVariant();
    }
}

QVariant SuspensionDTCModel::data(const QModelIndex &index, int role) const
{
    if (index.column() < 0 || index.column() >= m_colCount)
        return QVariant();

    if (index.row() >= m_node->dtcCount() && m_node->dtcCount()  > 0)
        return QVariant();

    CanDtc *dtc = m_node->getDtc(index.row());

    if (!dtc)
        return QVariant();

    if (role == CodeRole)
        return dtc->dtcCodeStr();
    else if (role == DescRole)
        return QVariant(dtc->dtcDescription());
    else if (role == OccurencesRole)
        return QVariant(dtc->occurences());
    else if (role == PendingRole)
        return QVariant(dtc->isPending());
    else
        return QVariant();
}

int SuspensionDTCModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_colCount;
}

int SuspensionDTCModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_node->dtcCount();
}

void SuspensionDTCModel::cleared(bool cleared, int count)
{
    Q_UNUSED(cleared)
    Q_UNUSED(count)
    beginResetModel();
    endResetModel();
//    if (cleared) {
//        beginRemoveRows(QModelIndex(), 0, count > 0 ? count -1 : 0);
//        endRemoveRows();
//    } else {
        // force refresh
//        emit dataChanged(QModelIndex(), QModelIndex());
//    }
}

void SuspensionDTCModel::updated(int idx)
{
    emit dataChanged(index(idx, 0), index(idx, m_colCount - 1));
}

void SuspensionDTCModel::added(int idx)
{
    beginInsertRows(QModelIndex(), idx, idx);
    endInsertRows();
}

QHash<int, QByteArray> SuspensionDTCModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CodeRole] = "code";
    roles[DescRole] = "desc";
    roles[OccurencesRole]  = "occurrences";
    roles[PendingRole] = "pending";
    return roles;
}
