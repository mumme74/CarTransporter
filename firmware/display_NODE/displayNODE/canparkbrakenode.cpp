#include "canparkbrakenode.h"
#include "can_protocol.h"
#include "park_settings.h"
#include "control.h"
#include <QQmlEngine>
#include <QDebug>

//#define TEST_DIAG 1

CanParkbrakeNode::CanParkbrakeNode(CanInterface *canInterface, QObject *parent) :
    CanAbstractNode(C_parkbrakeNode, canInterface, parent), m_inServiceState(false)
{
    // init some known PIDS
    QCanBusFrame pid1;
    pid1.setExtendedFrameFormat(false);
    pid1.setFrameId(CAN_MSG_TYPE_UPDATE | C_parkbrakeUpdPID_1 | C_parkbrakeNode);
    pid1.setPayload(QByteArray("\0\0\0\0\0\0\0\0", 8));

    QCanBusFrame pid2;
    pid2.setExtendedFrameFormat(false);
    pid2.setFrameId(CAN_MSG_TYPE_UPDATE | C_parkbrakeUpdPID_2 | C_parkbrakeNode);
    pid2.setPayload(QByteArray("\0\0\0\0\0\0\0\0", 8));

    QCanBusFrame pid3;
    pid3.setExtendedFrameFormat(false);
    pid3.setFrameId(CAN_MSG_TYPE_UPDATE | C_parkbrakeUpdPID_3 | C_parkbrakeNode);
    pid3.setPayload(QByteArray("\0\0\0\0\0", 5));

    QList<QCanBusFrame> frames;
    frames << pid1 << pid2 << pid3;

#ifdef TEST_DIAG
    // ------------- test ------------------------
    // for test
    QCanBusFrame dtcLength;
    dtcLength.setExtendedFrameFormat(false);
    dtcLength.setFrameId(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTCLength | C_parkbrakeNode);
    QByteArray pl;
    pl.insert(0, 1);
    dtcLength.setPayload(pl);

    QCanBusFrame dtc1;
    dtc1.setExtendedFrameFormat(false);
    dtc1.setFrameId(CAN_MSG_TYPE_EXCEPTION | C_parkbrakeExcNewDTC | C_parkbrakeNode);
    pl.clear();
    pl.insert(0, '\0');
    pl.insert(1, 0x30U);
    pl.insert(2, 0x10U);
    pl.insert(3, 7);
    pl.insert(4, 0x3AU);
    pl.insert(5, 0x42U);
    dtc1.setPayload(pl);

    frames << dtcLength << dtc1;

    // freezeFrames
    QCanBusFrame ff0;
    ff0.setExtendedFrameFormat(false);
    ff0.setFrameId(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTCFreezeFrame | C_parkbrakeNode);
    ff0.setPayload(QByteArray("\0\0\2\1\3\4", 6));

    QCanBusFrame ff1;
    ff1.setExtendedFrameFormat(false);
    ff1.setFrameId(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTCFreezeFrame | C_parkbrakeNode);
    ff1.setPayload(QByteArray("\0\1\2\1\3\4", 6));

    QCanBusFrame ff2;
    ff2.setExtendedFrameFormat(false);
    ff2.setFrameId(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTCFreezeFrame | C_parkbrakeNode);
    ff2.setPayload(QByteArray("\0\2\xF\xE\x6\xA", 6));

    QCanBusFrame ff3;
    ff3.setExtendedFrameFormat(false);
    ff3.setFrameId(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTCFreezeFrame | C_parkbrakeNode);
    ff3.setPayload(QByteArray("\0\3\x3\xE\xE\xF", 6));

    QCanBusFrame ff4;
    ff4.setExtendedFrameFormat(false);
    ff4.setFrameId(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTCFreezeFrame | C_parkbrakeNode);
    ff4.setPayload(QByteArray("\0\4\x3f\xE4\xE6\xF2\x20\xA5", 8));

    frames << ff0 << ff4 << ff2 << ff3 << ff1;

    // ------------ end test -------------------
#endif

    updatedFromCan(frames);

    qmlRegisterUncreatableType<CanParkbrakeNode>("mummesoft", 0, 1, "ParkbrakeNode", QStringLiteral("Cant create ParkbrakeNode from QML"));
    qmlRegisterUncreatableType<ParkbrakeConfig>("mummesoft", 0, 1, "ParkbrakeConfig", QStringLiteral("Cant create ParkbrakeConfig from QML"));

    // detect changes in state
    connect(getPid("LeftFront_state"), SIGNAL(valueChanged(const CanPid*)),
            this, SLOT(inServiceMode()));
    connect(getPid("RightFront_state"), SIGNAL(valueChanged(const CanPid*)),
            this, SLOT(inServiceMode()));
    connect(getPid("LeftRear_state"), SIGNAL(valueChanged(const CanPid*)),
            this, SLOT(inServiceMode()));
    connect(getPid("RightRear_state"), SIGNAL(valueChanged(const CanPid*)),
            this, SLOT(inServiceMode()));
}

CanParkbrakeNode::~CanParkbrakeNode()
{
}

bool CanParkbrakeNode::fetchDtc(int storedIdx, QJSValue jsCallback)
{
    return CanAbstractNode::fetchDtc(storedIdx, jsCallback, C_parkbrakeDiagDTC);
}

bool CanParkbrakeNode::fetchAllDtcs()
{
    return CanAbstractNode::fetchAllDtcs(C_parkbrakeDiagDTCLength);
}

bool CanParkbrakeNode::clearAllDtcs()
{
    return CanAbstractNode::clearAllDtcs(C_parkbrakeDiagClearDTCs);
}

bool CanParkbrakeNode::fetchFreezeFrame(int dtcNr, QJSValue jsCallback)
{
    return CanAbstractNode::fetchFreezeFrame(dtcNr, jsCallback, C_parkbrakeDiagDTCFreezeFrame);
}

bool CanParkbrakeNode::activateOutput(int wheel, bool tighten) const
{
    if (wheel < LeftFront || wheel > RightRear)
        return false;

    // request: t= tighten
    //  [0  -  1  -  2  -  3  -  4  -  5  -  6  -  7]
    //  t-LF t-RF   t-LR  t-RR  r-LF  r-RF  r-LR  r-RR
    // LeftFront == 0, RightRear == 3
    quint8 shiftDecr = tighten ? 0 : 4;
    shiftDecr -= wheel;
    quint8 msk = 1 << (7 - shiftDecr);

    QByteArray pl;
    pl.append(msk);
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagActuatorTest | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

bool CanParkbrakeNode::setServiceState(bool service)
{
    QCanBusFrame f(QCanBusFrame::RemoteRequestFrame);
    if (service)
        f.setFrameId(CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdServiceSet | C_displayNode);
    else
        f.setFrameId(CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdServiceUnset | C_displayNode);
    return m_canIface->sendFrame(f);
}

bool CanParkbrakeNode::fetchSetting(quint8 idx, QJSValue jsCallback)
{
    if (idx >= ParkbrakeConfig::S_EOF)
        return false;

    return CanAbstractNode::fetchSetting(idx, jsCallback, C_parkbrakeCmdGetConfig);
}

bool CanParkbrakeNode::setSettingUInt16(quint8 idx, quint16 vlu, QJSValue jsCallback)
{
    if (idx >= ParkbrakeConfig::S_EOF)
        return false;

    return CanAbstractNode::setSettingU16(idx, vlu, jsCallback, C_parkbrakeCmdSetConfig);
}



bool CanParkbrakeNode::inServiceMode()
{
    bool ret;
    CanPid *lf = getPid(tr("LeftFront_state")),
           *rf = getPid(tr("RightFront_state")),
           *lr = getPid(tr("LeftRear_state")),
           *rr = getPid(tr("RightRear_state"));
    if (!lf || !rf || !lr || !rr)
        return false;
    if (lf->valueInt() == InServiceState && rf->valueInt() == InServiceState &&
        lr->valueInt() == InServiceState && rr->valueInt() == InServiceState)
    {
        ret = true;
    } else
        ret = false;

    if (ret != m_inServiceState) {
        m_inServiceState = ret;
        emit serviceModeChanged(ret);
    }
    return ret;
}

void CanParkbrakeNode::updateCanFrame(const QCanBusFrame &frame)
{
    quint32 sid = frame.frameId();
    QByteArray data = frame.payload();
    quint16 msgId = sid & CAN_MSG_ID_MASK;

    // LF, RF, LR, RR
    // 0   1   2   3  <- byte orders
    switch (msgId) {
    case C_parkbrakeUpdPID_1:
        // state wheels brakes | revs / sec on wheels
        // [0:7][0:7][0:7][0:7] [0:7][0:7][0:7][0:7]
        //  LF   RF   LR   RR    LF    RF  LR   RR
        setStatePid(tr("LeftFront_state"), data[0], m_pids);
        setStatePid(tr("RightFront_state"), data[1], m_pids);
        setStatePid(tr("LeftRear_state"), data[2], m_pids);
        setStatePid(tr("RightRear_state"), data[3], m_pids);

        setWheelRevs(tr("LeftFront_revs"), data[4], m_pids);
        setWheelRevs(tr("RightFront_revs"), data[5], m_pids);
        setWheelRevs(tr("LeftRear_revs"), data[6], m_pids);
        setWheelRevs(tr("RightRear_revs"), data[7], m_pids);
        break;
    case C_parkbrakeUpdPID_2:
        // amps momentary      |  amps maximum this seq.
        // [0:7][0:7][0:7][0:7] [0:7][0:7][0:7][0:7]
        //  LF   RF   LR   RR    LF   RF   LR   RR
        setAmpPid(tr("LeftFront_motor"), data[0], m_pids, C_parkbrakeNode);
        setAmpPid(tr("RightFront_motor"), data[1], m_pids, C_parkbrakeNode);
        setAmpPid(tr("LeftRear_motor"), data[2], m_pids, C_parkbrakeNode);
        setAmpPid(tr("RightRear_motor"), data[3], m_pids, C_parkbrakeNode);
        // maximum
        setAmpPid(tr("LeftFront_motorMax"), data[4], m_pids, C_parkbrakeNode);
        setAmpPid(tr("RightFront_motorMax"), data[5], m_pids, C_parkbrakeNode);
        setAmpPid(tr("LeftRear_motorMax"), data[6], m_pids, C_parkbrakeNode);
        setAmpPid(tr("RightRear_motorMax"), data[7], m_pids, C_parkbrakeNode);

        break;
    case C_parkbrakeUpdPID_3: {
        // bat volt*1000|ign volt*1000|chip temp
        //   [0:15]         [0:15]    i[0:7] -> continue on next row

        // IGN|B.LIGHTS|BTN|BTN_INV|LF_LIM|RF_LIM|LR_LIM|RR_LIM
        // [7 -   6   -  5  -  4  -   3  -   2  -  1  -   0  ]
        //  bits so the whole data is 5bytes + 8bit = 6bytes
        quint16 vlu = (data[1] << 8) | (0x00ff & data[0]);
        setVoltPid(tr("Ignition volt"), vlu, m_pids, C_parkbrakeNode);

        vlu = (data[1] << 8) | (0x00ff & data[0]);
        setVoltPid(tr("Battery"), vlu, m_pids, C_parkbrakeNode);

        setTempPid(tr("ChipTemp"), data[4], m_pids, C_parkbrakeNode);

        setBoolPid(tr("Ignition"), data[5] & 0x80, m_pids, C_parkbrakeNode);
        setBoolPid(tr("Brakelights"), data[5] & 0x40, m_pids, C_parkbrakeNode);
        setBoolPid(tr("Button"), data[5] & 0x20, m_pids, C_parkbrakeNode);
        setBoolPid(tr("Button inv"), data[5] & 0x10, m_pids, C_parkbrakeNode);
        setBoolPid(tr("LF_currentLimit"), data[5] & 0x08, m_pids, C_parkbrakeNode);
        setBoolPid(tr("RF_currentLimit"), data[5] & 0x04, m_pids, C_parkbrakeNode);
        setBoolPid(tr("LR_currentLimit"), data[5] & 0x02, m_pids, C_parkbrakeNode);
        setBoolPid(tr("RR_currentLimit"), data[5] & 0x01, m_pids, C_parkbrakeNode);

     }   break;
    default:
        // unknown
        QString key = QString::number(sid, 16);
        QString vlu;
        for (quint8 v : data) {
            vlu += QString::number(v, 16);
        }
        setPidsValue(key, vlu, "", m_pids, C_parkbrakeNode);
        break;
    }
}

void CanParkbrakeNode::commandCanFrame(const QCanBusFrame &frame)
{
    // when parkbrakenode sends a command frame
    QByteArray payload = frame.payload();
    quint32 sid = frame.frameId();
    switch (sid & CAN_MSG_ID_MASK) {
    case C_parkbrakeCmdGetConfig: {
        quint8 idx = payload[0];
        quint16 vlu = (payload[2] << 8) | (0x00ff & payload[1]);
        settingsFetchArrival(idx, vlu);
    } break;
    case C_parkbrakeCmdSetConfig: {
        quint8 idx = payload[0];
        quint8 res = payload[1];
        settingsSetArrival(idx, res);

    } break;
    default:
        break;
    }
}

void CanParkbrakeNode::exceptionCanFrame(const QCanBusFrame &frame)
{
    // when parkbrakeNode sends a exceptionFrame
    QByteArray payload = frame.payload();
    quint32 sid = frame.frameId();
    switch (sid & CAN_MSG_ID_MASK) {
    case C_parkbrakeExcNewDTC: {
        // a new DTC has been set
        // broadcasted when a dtc is set
        // response
        //  [0:7]        [0:15]    [0:7]
        // stored nr     dtc code  occurrences
        quint16 code = (payload[2] << 8) | (0x00ff & payload[1]);
        CanDtc *dtc = getDtc(payload[0]);
        if (dtc == nullptr) {
            quint16 dtcTime = payload[5] << 8 | payload[4];
            dtc = new CanDtc(this, payload[0], code, payload[3], dtcTime);
            m_dtcs.insert(dtc->storedNr(), dtc);
            m_dtcCount++;
            emit dtcAdded(dtc->storedNr());
        } else {
            dtc->setOccurences(payload[3]);
            emit dtcUpdated(dtc->storedNr());
        }

    }   break;
    case C_parkbrakeExcUserError: {
        quint16 excId = (payload[1] << 8) | (0x00ff & payload[0]);
        if (excId == C_userErrorBrakeOff) {
            emit userErrorBrakeOff();
        } else if (excId == C_userErrorBtnInvOff) {
            emit userErrorBtnInvOff();
        } else if (excId == C_userErrorIgnOff) {
            emit userErrorIgnOff();
        }

        emit userError(excId);
    }   break;
    default:
        break;
    }
}

void CanParkbrakeNode::diagCanFrame(const QCanBusFrame &frame)
{
    // when parkbrakeNode sends a diagnose frame
    QByteArray payload = frame.payload();
    quint32 sid = frame.frameId();
    switch (sid & CAN_MSG_ID_MASK) {
    case C_parkbrakeDiagDTCLength:
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
                    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTC | C_displayNode, pl);
                    m_canIface->sendFrame(f);
                }
            }
        }
        break;

    case C_parkbrakeDiagClearDTCs:
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

    case C_parkbrakeDiagDTC: {
        // response:
        //  [0:7]        [0:15]    [0:7]        [0:15]
        // stored nr     dtc code  occurrences  time since startup when set
        int storedNr = payload[0];
        quint16 code = (payload[2] << 8) | (0x00ff & payload[1]);
        quint16 time = (payload[5] << 8) | (0x00ff & payload[4]);
        if (m_dtcs.contains(storedNr)) {
            CanDtc *dtc = m_dtcs[storedNr];
            dtc->setOccurences(payload[3]);
            emit dtcUpdated(storedNr);
        } else {
            CanDtc *dtc = new CanDtc(this, storedNr, code, payload[3], time);
            m_dtcs.insert(storedNr, dtc);
            emit dtcAdded(storedNr);
        }
        dtcArriveDoQmlCallback(storedNr);

    }   break;
    case C_parkbrakeDiagDTCFreezeFrame: {
        // response:
        //  [0:7]        [0:7]      [0:48]
        // stored nr    frame nr    data payload
        //           /- frame0:     [0:7][0:7][0:7][0:7]
        //          /    state brakes LF  RF   LR    RR
        //     pid1:\   frame1:     [0:7][0:7][0:7][0:7]
        //           \-  wheelspeed   LF   RF   LR   RR
        //           /- frame2:     [0:7][0:7][0:7][0:7]
        //          /    amps mom.    LF   RF  LR   RR
        //     pid2:\   frame3:     [0:7][0:7][0:7][0:7]
        //           \-  amps max.    LF   RF  LR   RR
        //     pid3:    frame4:     [0:15][0:15][0:7] [0-7]
        //                           batV  ignV  temp  input states
        //     last byte frame 4:
        //                [ 7 -   6   -  5  -  4  -   3  -   2  -  1  -   0  ]
        //                IGN|B.LIGHTS|BTN|BTN_INV|LF_LIM|RF_LIM|LR_LIM|RR_LIM

        quint8 dtcNr = payload.at(0);
        CanFreezeFrame *ff = nullptr;
        if (!m_freezeFrames.contains(dtcNr)) {
            // not yet created a chace for this ff
            ff = new CanFreezeFrame(this, dtcNr);
            m_freezeFrames.insert(dtcNr, ff);
        } else {
            // get the stored freezeFrame Object
            ff = m_freezeFrames[dtcNr];
        }


        quint8 frameNr = payload.at(1);

        if (frameNr == 0) {
            setStatePid("LeftFront_state", payload[2], ff->m_pids);
            setStatePid("RightFront_state", payload[3], ff->m_pids);
            setStatePid("LeftRear_state", payload[4], ff->m_pids);
            setStatePid("RightRear_state", payload[5], ff->m_pids);
        } else if (frameNr == 1) {
            setWheelRevs("LeftFront_rev", payload[2], ff->m_pids);
            setWheelRevs("RightFront_rev", payload[3], ff->m_pids);
            setWheelRevs("LeftRear_rev", payload[4], ff->m_pids);
            setWheelRevs("RightRear_rev", payload[5], ff->m_pids);
        } else if (frameNr == 2) {
            setAmpPid("LeftFront_motor", payload[2], ff->m_pids, C_parkbrakeNode);
            setAmpPid("RightFront_motor", payload[3], ff->m_pids, C_parkbrakeNode);
            setAmpPid("LeftRear_motor", payload[4], ff->m_pids, C_parkbrakeNode);
            setAmpPid("RightRear_motor", payload[5], ff->m_pids, C_parkbrakeNode);
        } else if (frameNr == 3) {
            setAmpPid("LeftFront_motorMax", payload[2], ff->m_pids, C_parkbrakeNode);
            setAmpPid("RightFront_motorMax", payload[3], ff->m_pids, C_parkbrakeNode);
            setAmpPid("LeftRear_motorMax", payload[4], ff->m_pids, C_parkbrakeNode);
            setAmpPid("RightRear_motorMax", payload[5], ff->m_pids, C_parkbrakeNode);
        } else if (frameNr == 4) {
            quint16 vlu = (payload[3] << 8) | (0x00ff & payload[2]);
            setVoltPid(tr("Ignition volt"), vlu, ff->m_pids, C_parkbrakeNode);

            vlu = (payload[5] << 8) | (0x00ff & payload[4]);
            setVoltPid(tr("Battery"), vlu, ff->m_pids, C_parkbrakeNode);

            setTempPid(tr("ChipTemp"), payload[6], ff->m_pids, C_parkbrakeNode);

            setBoolPid(tr("Ignition"), payload[7] & 0x80, ff->m_pids, C_parkbrakeNode);
            setBoolPid(tr("Brakelights"), payload[7] & 0x40, ff->m_pids, C_parkbrakeNode);
            setBoolPid(tr("Button"), payload[7] & 0x20, ff->m_pids, C_parkbrakeNode);
            setBoolPid(tr("Button inv"), payload[7] & 0x10, ff->m_pids, C_parkbrakeNode);
            setBoolPid(tr("LF_currentLimit"), payload[7] & 0x08, ff->m_pids, C_parkbrakeNode);
            setBoolPid(tr("RF_currentLimit"), payload[7] & 0x04, ff->m_pids, C_parkbrakeNode);
            setBoolPid(tr("LR_currentLimit"), payload[7] & 0x02, ff->m_pids, C_parkbrakeNode);
            setBoolPid(tr("RR_currentLimit"), payload[7] & 0x01, ff->m_pids, C_parkbrakeNode);
        }

        // maybe we have all frames?
        if (ff->pidCount() >= 27) {
            ff->setFinished(true);
            freezeFrameArrival(dtcNr);
        }


    }   break;
    case C_parkbrakeDiagActuatorTest:
        // not sure if we should do anything here
        break;
    }
}


void CanParkbrakeNode::setStatePid(QString key, quint8 state, PidStore &pidStore)
{
    QString value;
    switch (static_cast<ctrl_states>(state)) {
    case Tightened:
        value = tr("Tightened");
        break;
    case Tightening:
        value = tr("Tightening");
        break;
    case Released:
        value = tr("Released");
        break;
    case Releasing:
        value = tr("Releasing");
        break;
    case InServiceState:
        value = tr("ServiceState");
        break;
    case SetServiceState:
        value = tr("ToServiceState");
        break;
    case ErrorState: // fallthrough
    default:
        value = tr("ErrorState");
        break;
    }

    setPidsValue(key, value, "", pidStore, C_parkbrakeNode);
}

void CanParkbrakeNode::setWheelRevs(QString key, quint8 rev, PidStore &pidStore)
{
    QString value = QString::number(rev);
    setPidsValue(key, value, "r/s", pidStore, C_parkbrakeNode);
}

// -------------------------------------------------------------

ParkbrakeDTCModel::ParkbrakeDTCModel(CanParkbrakeNode *node) :
    QAbstractTableModel(node),
    m_node(node)
{
    connect(m_node, SIGNAL(dtcUpdated(int)), this, SLOT(updated(int)));
    connect(m_node, SIGNAL(dtcAdded(int)), this, SLOT(added(int)));
    connect(m_node, SIGNAL(dtcsCleared(bool, int)), this, SLOT(cleared(bool, int)));

    qmlRegisterUncreatableType<ParkbrakeDTCModel>("mummesoft", 0, 1, "ParkbrakeDTCModel", QStringLiteral("Cant create ParkbrakeDTCModel from QML"));
}

ParkbrakeDTCModel::~ParkbrakeDTCModel()
{
}

QVariant ParkbrakeDTCModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant ParkbrakeDTCModel::data(const QModelIndex &index, int role) const
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
    else if (role == TimeRole)
        return QVariant(dtc->timeSinceStartup());
    else
        return QVariant();
}

int ParkbrakeDTCModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_colCount;
}

int ParkbrakeDTCModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_node->dtcCount();
}

void ParkbrakeDTCModel::cleared(bool cleared, int count)
{
    Q_UNUSED(cleared)
    Q_UNUSED(count)
//    beginResetModel();
//    endResetModel();
    if (cleared) {
        beginRemoveRows(QModelIndex(), 0, count > 0 ? count -1 : 0);
        endRemoveRows();
    } else {
        // force refresh
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

void ParkbrakeDTCModel::updated(int idx)
{
    emit dataChanged(index(idx, 0), index(idx, m_colCount - 1));
}

void ParkbrakeDTCModel::added(int idx)
{
    beginInsertRows(QModelIndex(), idx, idx);
    endInsertRows();
}

QHash<int, QByteArray> ParkbrakeDTCModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CodeRole] = "code";
    roles[DescRole] = "desc";
    roles[OccurencesRole]  = "occurrences";
    roles[TimeRole] = "time";
    return roles;
}

