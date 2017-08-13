#include "canparkbrakenode.h"
#include "can_protocol.h"
#include "park_settings.h"
#include "control.h"
#include <QQmlEngine>
#include <QDebug>

//#define TEST_DIAG 1

CanParkbrakeNode::CanParkbrakeNode(CanInterface *canInterface, QObject *parent) :
    CanAbstractNode(canInterface, parent), m_inServiceState(false)
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
    dtc1.setFrameId(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTC | C_parkbrakeNode);
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

    qmlRegisterUncreatableType<CanParkbrakeNode>("mummesoft", 0, 1, "parkbrakeNode", QStringLiteral("Cant create CanParkbrakeNode from QML"));
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

bool CanParkbrakeNode::hasProperty(const QString &key)
{
    return m_pids.keys().contains(key);
}

bool CanParkbrakeNode::fetchDtc(int storedIdx, QJSValue jsCallback)
{
    if (storedIdx < 0 || storedIdx > m_dtcCount)
        return false;

    if (!jsCallback.isCallable())
        return false;

    // store callback in stack for later use
    m_dtcFetchCallback.insert(storedIdx, jsCallback);

    // already fetched
    if (m_dtcs.contains(storedIdx)) {
        dtcOnArrival(storedIdx);
        return true;
    }

    // get from Canbus
    QByteArray payload;
    payload.insert(0, (storedIdx & 0xFF));
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTC | C_displayNode, payload);
    f.setExtendedFrameFormat(false);
    m_canIface->sendFrame(f);

    return true;
}

void CanParkbrakeNode::fetchAllDtcs()
{
    // first reset dtcCount then fetch the new dtcCount
    // when that frame arrives it will automatically retrieve these DTCs
    m_dtcCount = 0;
    // must be remote request frame
    QCanBusFrame f(QCanBusFrame::RemoteRequestFrame);
    f.setFrameId(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTCLength | C_displayNode);
    m_canIface->sendFrame(f);
}

void CanParkbrakeNode::clearAllDtcs()
{
    // gets cleared when reponse frame arrives
    QByteArray pl(m_dtcCount, 1);
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagClearDTCs | C_displayNode, pl);
    f.setExtendedFrameFormat(false);
    m_canIface->sendFrame(f);
}

bool CanParkbrakeNode::fetchFreezeFrame(int dtcNr, QJSValue jsCallback)
{
    if (dtcNr < 0 || dtcNr > m_dtcCount)
        return false;

    if (!jsCallback.isCallable())
        return false;


    // already fetched?
    if (m_freezeFrames.contains(dtcNr)) {
        if (m_freezeFrames[dtcNr]->isFinished()) {
            m_freezeFrameFetchCallback.insert(dtcNr, jsCallback);
            freezeFrameArrival(dtcNr);
            return true;
        }
        // we are waiting for more frames here
        return true;
    }

    // we havent yet tried to fetch this frame

    // store in stack for later use
    m_freezeFrameFetchCallback.insert(dtcNr, jsCallback);

    // create a freezeFrame object for this dtc,
    // used later to check against multiple fetches
    CanFreezeFrame *ff = new CanFreezeFrame(this, dtcNr);
    m_freezeFrames.insert(dtcNr, ff);

    // get from can
    QByteArray pl(dtcNr, 1);
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTCFreezeFrame | C_displayNode, pl);
    f.setExtendedFrameFormat(false);
    m_canIface->sendFrame(f);
    return true;
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

    QByteArray pl(msk, 1);
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagActuatorTest | C_displayNode, pl);
    f.setExtendedFrameFormat(false);
    m_canIface->sendFrame(f);
    return true;
}

void CanParkbrakeNode::setServiceState(bool service)
{
    QCanBusFrame f(QCanBusFrame::RemoteRequestFrame);
    if (service)
        f.setFrameId(CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdServiceSet | C_displayNode);
    else
        f.setFrameId(CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdServiceUnset | C_displayNode);
    f.setExtendedFrameFormat(false);
    m_canIface->sendFrame(f);
}

bool CanParkbrakeNode::fetchSetting(quint8 idx, QJSValue jsCallback)
{
    if (idx >= S_EOF)
        return false;

    if (!jsCallback.isCallable())
        return false;

    m_settingsFetchCallback.insert(idx, jsCallback);

    QByteArray pl(idx & 0xFF, 1);
    QCanBusFrame f(CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdGetConfig | C_displayNode, pl);
    f.setExtendedFrameFormat(false);
    m_canIface->sendFrame(f);
    return true;
}

bool CanParkbrakeNode::setSetting(quint8 idx, quint16 vlu, QJSValue jsCallback)
{
    if (idx >= S_EOF)
        return false;

    if (!jsCallback.isCallable())
        return false;

    m_settingsSetCallback.insert(idx, jsCallback);
    QByteArray pl("\0\0\0", 3);
    pl[0] = idx;
    pl[1] = vlu & 0xFF;
    pl[2] = (vlu & 0xFF00) >> 8;
    QCanBusFrame f(CAN_MSG_TYPE_COMMAND | C_parkbrakeCmdSetConfig | C_displayNode, pl);
    f.setExtendedFrameFormat(false);
    m_canIface->sendFrame(f);
    return true;
}



bool CanParkbrakeNode::inServiceMode()
{
    bool ret;
    CanPid *lf = getPid("LeftFront_state"),
           *rf = getPid("RightFront_state"),
           *lr = getPid("LeftRear_state"),
           *rr = getPid("RightRear_state");
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

void CanParkbrakeNode::updatedFromCan(QList<QCanBusFrame> &frames)
{
    if (frames.size() > 0) {
        for (const QCanBusFrame &frame : frames) {
            quint32 sid = frame.frameId();
            if ((sid & CAN_MSG_SENDER_ID_MASK) != C_parkbrakeNode)
                continue;

            switch (sid & CAN_MSG_TYPE_MASK) {
            case CAN_MSG_TYPE_UPDATE:
                updateCanFrame(frame);
                break;
            case CAN_MSG_TYPE_COMMAND:
                commandCanFrame(frame);
                break;
            case CAN_MSG_TYPE_DIAG:
                diagCanFrame(frame);
                break;
            case CAN_MSG_TYPE_EXCEPTION:
                exceptionCanFrame(frame);
                break;
            default:
                continue;
                break;
            }
        }
    }
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
        setAmpPid(tr("LeftFront_motor"), data[0], m_pids);
        setAmpPid(tr("RightFront_motor"), data[1], m_pids);
        setAmpPid(tr("LeftRear_motor"), data[2], m_pids);
        setAmpPid(tr("RightRear_motor"), data[3], m_pids);
        // maximum
        setAmpPid(tr("LeftFront_motorMax"), data[4], m_pids);
        setAmpPid(tr("RightFront_motorMax"), data[5], m_pids);
        setAmpPid(tr("LeftRear_motorMax"), data[6], m_pids);
        setAmpPid(tr("RightRear_motorMax"), data[7], m_pids);

        break;
    case C_parkbrakeUpdPID_3: {
        // bat volt*1000|ign volt*1000|chip temp
        //   [0:15]         [0:15]    i[0:7] -> continue on next row

        // IGN|B.LIGHTS|BTN|BTN_INV|LF_LIM|RF_LIM|LR_LIM|RR_LIM
        // [7 -   6   -  5  -  4  -   3  -   2  -  1  -   0  ]
        //  bits so the whole data is 5bytes + 8bit = 6bytes
        quint16 vlu = (data[1] << 8) | data[0];
        setVoltPid(tr("Ignition volt"), vlu, m_pids);

        vlu = (data[1] << 8) | data[0];
        setVoltPid(tr("Battery"), vlu, m_pids);

        setTempPid(tr("ChipTemp"), data[4], m_pids);

        setBoolPid(tr("Ignition"), data[5] & 0x80, m_pids);
        setBoolPid(tr("Brakelights"), data[5] & 0x40, m_pids);
        setBoolPid(tr("Button"), data[5] & 0x20, m_pids);
        setBoolPid(tr("Button inv"), data[5] & 0x10, m_pids);
        setBoolPid(tr("LF_currentLimit"), data[5] & 0x08, m_pids);
        setBoolPid(tr("RF_currentLimit"), data[5] & 0x04, m_pids);
        setBoolPid(tr("LR_currentLimit"), data[5] & 0x02, m_pids);
        setBoolPid(tr("RR_currentLimit"), data[5] & 0x01, m_pids);

     }   break;
    default:
        // unknown
        QString key = QString::number(sid, 16);
        QString vlu;
        for (quint8 v : data) {
            vlu += QString::number(v, 16);
        }
        setPidsValue(key, vlu, "", m_pids);
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
        quint16 vlu = (payload[2] << 8) | payload[1];
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
        quint16 code = (payload[1] << 8) | payload[0];
        CanDtc *dtc = getDtc(payload[0]);
        if (dtc == nullptr) {
            dtc = new CanDtc(this, payload[0], code, payload[3], 0);
            m_dtcs.insert(dtc->storedNr(), dtc);
            emit dtcArrived(dtc->storedNr());
        } else {
            dtc->setOccurences(payload[3]);
        }
        emit newDtcSet(dtc);

    }   break;
    case C_parkbrakeExcUserError: {
        quint16 excId = (payload.at(1) << 8) | payload.at(0);
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
                    QByteArray pl(i, 1);
                    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_parkbrakeDiagDTC | C_displayNode, pl);
                    f.setExtendedFrameFormat(false);
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
            bool cleared = (quint8)(payload[0]) != 0;
            if (!cleared)
                cleared = m_dtcCount == 0; // when clearing a empty dtcs list
            emit dtcsCleared(cleared);
            emit dtcCountChanged(m_dtcCount);

        }

        break;

    case C_parkbrakeDiagDTC: {
        // response:
        //  [0:7]        [0:15]    [0:7]        [0:15]
        // stored nr     dtc code  occurrences  time since startup when set
        int storedNr = payload[0];
        quint16 code = (payload[2] << 8) | payload[1];
        quint16 time = (payload[5] << 8) | payload[4];
        if (m_dtcs.contains(storedNr)) {
            CanDtc *dtc = m_dtcs[storedNr];
            dtc->setOccurences(payload[3]);
        } else {
            CanDtc *dtc = new CanDtc(this, storedNr, code, payload[3], time);
            m_dtcs.insert(storedNr, dtc);
        }
        dtcOnArrival(storedNr);
        emit dtcArrived(storedNr);

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
            setAmpPid("LeftFront_motor", payload[2], ff->m_pids);
            setAmpPid("RightFront_motor", payload[3], ff->m_pids);
            setAmpPid("LeftRear_motor", payload[4], ff->m_pids);
            setAmpPid("RightRear_motor", payload[5], ff->m_pids);
        } else if (frameNr == 3) {
            setAmpPid("LeftFront_motorMax", payload[2], ff->m_pids);
            setAmpPid("RightFront_motorMax", payload[3], ff->m_pids);
            setAmpPid("LeftRear_motorMax", payload[4], ff->m_pids);
            setAmpPid("RightRear_motorMax", payload[5], ff->m_pids);
        } else if (frameNr == 4) {
            quint16 vlu = (payload[3] << 8) | payload[2];
            setVoltPid(tr("Ignition volt"), vlu, ff->m_pids);

            vlu = (payload[5] << 8) | payload[4];
            setVoltPid(tr("Battery"), vlu, ff->m_pids);

            setTempPid(tr("ChipTemp"), payload[6], ff->m_pids);

            setBoolPid(tr("Ignition"), payload[7] & 0x80, ff->m_pids);
            setBoolPid(tr("Brakelights"), payload[7] & 0x40, ff->m_pids);
            setBoolPid(tr("Button"), payload[7] & 0x20, ff->m_pids);
            setBoolPid(tr("Button inv"), payload[7] & 0x10, ff->m_pids);
            setBoolPid(tr("LF_currentLimit"), payload[7] & 0x08, ff->m_pids);
            setBoolPid(tr("RF_currentLimit"), payload[7] & 0x04, ff->m_pids);
            setBoolPid(tr("LR_currentLimit"), payload[7] & 0x02, ff->m_pids);
            setBoolPid(tr("RR_currentLimit"), payload[7] & 0x01, ff->m_pids);
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
    case ErrorState:
        value = tr("ErrorState");
        break;
    }

    setPidsValue(key, value, "", pidStore);
}

void CanParkbrakeNode::setWheelRevs(QString key, quint8 rev, PidStore &pidStore)
{
    QString value = QString::number(rev);
    setPidsValue(key, value, "r/s", pidStore);
}

