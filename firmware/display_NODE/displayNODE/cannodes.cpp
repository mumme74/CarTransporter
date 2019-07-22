#include "cannodes.h"
#include "can_protocol.h"
#include <QQmlEngine>
#include <QDebug>


//#define TEST_DIAG 1

// -------------------------------------------------------------------------------


CanDtc::CanDtc() :
    QObject(),
    m_storedNr(-1),
    m_dtcCode(-1),
    m_occurences(-1),
    m_timeSinceStartup(-1),
    m_pending(false)
{
}

CanDtc::CanDtc(const CanDtc &other) :
    QObject(other.parent()),
    m_storedNr(other.m_storedNr),
    m_dtcCode(other.m_dtcCode),
    m_occurences(other.m_occurences),
    m_timeSinceStartup(other.m_timeSinceStartup),
    m_pending(other.m_pending)
{
}

CanDtc::CanDtc(QObject *parent, int storedNr, int dtcCode,
               int occurences, int timeSinceStartup) :
    QObject(parent),
    m_storedNr(storedNr), m_dtcCode(dtcCode),
    m_occurences(occurences), m_timeSinceStartup(timeSinceStartup),
    m_pending(false)
{
}

CanDtc::~CanDtc()
{
}

int CanDtc::storedNr() const
{
    return m_storedNr;
}

int CanDtc::dtcCode() const
{
    return m_dtcCode;
}

const QString CanDtc::dtcCodeStr() const
{
    return QString("C%1").arg(QString::number(m_dtcCode, 16));
}

const QString CanDtc::dtcDescription() const
{
    QString desc;
    switch (static_cast<can_DTCs_e>(m_dtcCode)) {
    case C_dtc_LF_wheelsensor:
        desc = tr("Left front wheelsensor circuit"); break;
    case C_dtc_RF_wheelsensor:
        desc = tr("Right front wheelsensor circuit"); break;
    case C_dtc_LR_wheelsensor:
        desc = tr("Left rear wheelsensor circuit"); break;
    case C_dtc_RR_wheelsensor:
        desc = tr("Right rear wheelsensor circuit"); break;

    case C_dtc_ADC_error_LF_release:
        desc = tr("ADC error when LF release"); break;
    case C_dtc_ADC_error_RF_release:
        desc = tr("ADC error when RF release"); break;
    case C_dtc_ADC_error_LR_release:
        desc = tr("ADC error when LR release"); break;
    case C_dtc_ADC_error_RR_release:
        desc = tr("ADC error when RR release"); break;

    case C_dtc_OverCurrent_LF_release:
        desc = tr("Overcurrent when LF release"); break;
    case C_dtc_OverCurrent_RF_release:
        desc = tr("Overcurrent when RF release"); break;
    case C_dtc_OverCurrent_LR_release:
        desc = tr("Overcurrent when LR release"); break;
    case C_dtc_OverCurrent_RR_release:
        desc = tr("Overcurrent when RR release"); break;

    case C_dtc_ADC_error_LF_tighten:
        desc = tr("ADC error when LF tighten"); break;
    case C_dtc_ADC_error_RF_tighten:
        desc = tr("ADC error when RF tighten"); break;
    case C_dtc_ADC_error_LR_tighten:
        desc = tr("ADC error when LR tighten"); break;
    case C_dtc_ADC_error_RR_tighten:
        desc = tr("ADC error when RR tighten"); break;

    case C_dtc_OverCurrent_LF_tighten:
        desc = tr("Overcurrent when LF tighten"); break;
    case C_dtc_OverCurrent_RF_tighten:
        desc = tr("Overcurrent when RF tighten"); break;
    case C_dtc_OverCurrent_LR_tighten:
        desc = tr("Overcurrent when LR tighten"); break;
    case C_dtc_OverCurrent_RR_tighten:
        desc = tr("Overcurrent when RR tighten"); break;

    case C_dtc_ADC_error_LF_service:
        desc = tr("ADC error when LF service"); break;
    case C_dtc_ADC_error_RF_service:
        desc = tr("ADC error when RF service"); break;
    case C_dtc_ADC_error_LR_service:
        desc = tr("ADC error when LR service"); break;
    case C_dtc_ADC_error_RR_service:
        desc = tr("ADC error when RR service"); break;

    case C_dtc_OverCurrent_LF_service:
        desc = tr("Overcurrent when LF service"); break;
    case C_dtc_OverCurrent_RF_service:
        desc = tr("Overcurrent when RF service"); break;
    case C_dtc_OverCurrent_LR_service:
        desc = tr("Overcurrent when LR service"); break;
    case C_dtc_OverCurrent_RR_service:
        desc = tr("Overcurrent when RR service"); break;
    case C_dtc_parkbrake_EEProm_Error:
        desc = tr("EEprom error in node"); break;
    case C_dtc_parkbrake_internal_Error:
        desc = tr("Internal error in node"); break;

    // suspension outputs
    case C_dtc_leftFill_openLoad:
        desc = tr("Leftfill open or positive short"); break;
    case C_dtc_leftFill_shorted:
        desc = tr("Leftfill negative short"); break;
    case C_dtc_leftFill_overtemp:
        desc = tr("Leftfill output overtemp"); break;
    case C_dtc_leftFill_overload:
        desc = tr("Leftfill output overload"); break;
    case C_dtc_leftFill_hardwarefault:
        desc = tr("Leftfill output hardwarfault"); break;

    case C_dtc_leftDump_openLoad:
        desc = tr("Leftdump open or positive short"); break;
    case C_dtc_leftDump_shorted:
        desc = tr("Leftdump negative short"); break;
    case C_dtc_leftDump_overtemp:
        desc = tr("Leftdump output overtemp"); break;
    case C_dtc_leftDump_overload:
        desc = tr("Leftdump output overload"); break;
    case C_dtc_leftDump_hardwarefault:
        desc = tr("Leftdump output hardwarfault"); break;

    case C_dtc_leftSuck_openLoad:
        desc = tr("Leftsuck open or positive short"); break;
    case C_dtc_leftSuck_shorted:
        desc = tr("Leftsuck negative short"); break;
    case C_dtc_leftSuck_overtemp:
        desc = tr("Leftsuck output overtemp"); break;
    case C_dtc_leftSuck_overload:
        desc = tr("Leftsuck output overload"); break;
    case C_dtc_leftSuck_hardwarefault:
        desc = tr("Leftsuck output hardwarfault"); break;

    case C_dtc_rightFill_openLoad:
        desc = tr("Rightfill open or positive short"); break;
    case C_dtc_rightFill_shorted:
        desc = tr("Rightfill negative short"); break;
    case C_dtc_rightFill_overtemp:
        desc = tr("Rightfill output overtemp"); break;
    case C_dtc_rightFill_overload:
        desc = tr("Rightfill output overload"); break;
    case C_dtc_rightFill_hardwarefault:
        desc = tr("Rightfill output hardwarfault"); break;

    case C_dtc_rightDump_openLoad:
        desc = tr("Rightdump open or positive short"); break;
    case C_dtc_rightDump_shorted:
        desc = tr("Rightdump negative short"); break;
    case C_dtc_rightDump_overtemp:
        desc = tr("Rightdump output overtemp"); break;
    case C_dtc_rightDump_overload:
        desc = tr("Rightdump output overload"); break;
    case C_dtc_rightDump_hardwarefault:
        desc = tr("Rightdump output hardwarfault"); break;

    case C_dtc_rightSuck_openLoad:
        desc = tr("Rightsuck open or positive short"); break;
    case C_dtc_rightSuck_shorted:
        desc = tr("Rightsuck negative short"); break;
    case C_dtc_rightSuck_overtemp:
        desc = tr("Rightsuck output overtemp"); break;
    case C_dtc_rightSuck_overload:
        desc = tr("Rightsuck output overload"); break;
    case C_dtc_rightSuck_hardwarefault:
        desc = tr("Rightsuck output hardwarfault"); break;

    case C_dtc_airDryer_openLoad:
        desc = tr("Airdryer open or positive short"); break;
    case C_dtc_airDryer_shorted:
        desc = tr("Airdryer negative short"); break;
    case C_dtc_airDryer_overtemp:
        desc = tr("Airdryer output overtemp"); break;
    case C_dtc_airDryer_overload:
        desc = tr("Airdryer output overload"); break;
    case C_dtc_airDryer_hardwarefault:
        desc = tr("Airdryer output hardwarfault"); break;

    case C_dtc_spare1_openLoad:
        desc = tr("Spare1 open or positive short"); break;
    case C_dtc_spare1_shorted:
        desc = tr("Spare1 negative short"); break;
    case C_dtc_spare1_overtemp:
        desc = tr("Spare1 output overtemp"); break;
    case C_dtc_spare1_overload:
        desc = tr("Spare1 output overload"); break;
    case C_dtc_spare1_hardwarefault:
        desc = tr("Spare1 output hardwarfault"); break;

    case C_dtc_compressor_openLoad:
        desc = tr("Compressor open or positive short"); break;
    case C_dtc_compressor_shorted:
        desc = tr("Compressor negative short"); break;
    case C_dtc_compressor_overtemp:
        desc = tr("Compressor output overtemp"); break;
    case C_dtc_compressor_overload:
        desc = tr("Compressor output overload"); break;
    case C_dtc_compressor_hardwarefault:
        desc = tr("Compressor output hardwarfault"); break;

    // suspension inputs
    case C_dtc_airPressure_nonValidValue:
        desc = tr("airpressure invalid value"); break;
    case C_dtc_airPressure_tooLowValue:
        desc =tr("airpressure value too low"); break;
    case C_dtc_airPressure_tooHighValue:
        desc = tr("airpressure value too high"); break;
    case C_dtc_airPressure_tooFastFalling:
        desc = tr("airpressure value falls too fast"); break;
    case C_dtc_airPressure_tooFastRising:
        desc = tr("airpressure value rises too fast"); break;

    case C_dtc_leftPressure_nonValidValue:
        desc = tr("left pressure invalid value"); break;
    case C_dtc_leftPressure_tooLowValue:
        desc = tr("left pressure too low"); break;
    case C_dtc_leftPressure_tooHighValue:
        desc = tr("left pressure too high"); break;
    case C_dtc_leftPressure_tooFastFalling:
        desc = tr("left pressure falls too fast"); break;
    case C_dtc_leftPressure_tooFastRising:
        desc = tr("left pressure rises too fast"); break;

    case C_dtc_leftHeight_nonValidValue:
        desc = tr("left height value invalid"); break;
    case C_dtc_leftHeight_tooLowValue:
        desc = tr("left height value too low"); break;
    case C_dtc_leftHeight_tooHighValue:
        desc = tr("left height value too high"); break;
    case C_dtc_leftHeight_tooFastFalling:
        desc = tr("left height value falls too fast"); break;
    case C_dtc_leftHeight_tooFastRising:
        desc = tr("left height value rises too fast"); break;

    case C_dtc_rightPressure_nonValidValue:
        desc = tr("right pressure invalid value"); break;
    case C_dtc_rightPressure_tooLowValue:
        desc = tr("right pressure too low"); break;
    case C_dtc_rightPresure_tooHighValue:
        desc = tr("right pressure too high"); break;
    case C_dtc_rightPresure_tooFastFalling:
        desc = tr("right pressure falls too fast"); break;
    case C_dtc_rightPresure_tooFastRising:
        desc = tr("right pressure rises too fast"); break;

    case C_dtc_rightHeight_nonValidValue:
        desc = tr("right height invalid value"); break;
    case C_dtc_rightHeight_tooLowValue:
        desc = tr("right height too low value"); break;
    case C_dtc_rightHeight_tooHighValue:
        desc = tr("right height too high value"); break;
    case C_dtc_rightHeight_tooFastFalling:
        desc = tr("right height value falls too fast"); break;
    case C_dtc_rightHeight_tooFastRising:
        desc = tr("right height value rises too fast"); break;

    case C_dtc_compressorTemp_nonValidValue:
        desc = tr("compressortemp invalid value"); break;
    case C_dtc_compressorTemp_tooLowValue:
        desc = tr("compressortemp value too low"); break;
    case C_dtc_compressorTemp_tooHighValue:
        desc = tr("compressortemp value too high"); break;
    case C_dtc_compressorTemp_tooFastFalling:
        desc = tr("compressortemp value falls too fast"); break;
    case C_dtc_compressorTemp_tooFastRising:
        desc = tr("compressortemp value rises too fast"); break;

    case C_dtc_systemPressure_nonValidValue:
        desc = tr("systempressure invalid value"); break;
    case C_dtc_systemPressure_tooLowValue:
        desc =tr("systempressure value too low"); break;
    case C_dtc_systemPressure_tooHighValue:
        desc = tr("systempressure value too high"); break;
    case C_dtc_systemPressure_tooFastFalling:
        desc = tr("systempressure value falls too fast"); break;
    case C_dtc_systemPressure_tooFastRising:
        desc = tr("systempressure value rises too fast"); break;

    case C_dtc_spareAnalog1_nonValidValue:
        desc = tr("spareAnalog1 invalid value"); break;
    case C_dtc_spareAnalog1_tooLowValue:
        desc = tr("spareAnalog1 too low value"); break;
    case C_dtc_spareAnalog1_tooHighValue:
        desc = tr("spareAnalog1 too high value"); break;
    case C_dtc_spareAnalog1_tooFastFalling:
        desc = tr("spareAnalog1 value falls too fast"); break;
    case C_dtc_spareAnalog1_tooFastRising:
        desc = tr("spareAnalog1 value rises too fast"); break;

    case C_dtc_spareTemp1_nonValidValue:
        desc = tr("spareTemp1 invalid value"); break;
    case C_dtc_spareTemp1_tooLowValue:
        desc = tr("spareTemp1 value too low"); break;
    case C_dtc_spareTemp1_tooHighValue:
        desc = tr("spareTemp1 value too high"); break;
    case C_dtc_spareTemp1_tooFastFalling:
        desc = tr("spareTemp1 value falls too fast"); break;
    case C_dtc_spareTemp1_tooFastRising:
        desc = tr("spareTemp1 value rises too fast"); break;

    case C_dtc_airFeed_overCurrent:
        desc = tr("Airfeed overcurrent"); break;
    case C_dtc_airFeed_overHeated:
        desc = tr("Airfeed overheated"); break;
    case C_dtc_suspension_EEProm_Error:
        desc = tr("Suspension EEProm error in node"); break;
    case C_dtc_suspension_internal_Error:
        desc = tr("Suspension internal error in node"); break;

    default:
        desc = tr("Unknown DTC"); break;
    }

    return desc;
}

int CanDtc::occurences() const
{
    return m_occurences;
}

void CanDtc::setOccurences(int occurences)
{
    if (occurences == m_occurences)
        return;

    m_occurences = occurences;
    emit occurencesChanged();
}

int CanDtc::timeSinceStartup() const
{
    return m_timeSinceStartup;
}

bool CanDtc::isPending() const
{
    return m_pending;
}

void CanDtc::setPending(bool pending)
{
    if (pending == m_pending)
        return;

    m_pending = pending;
    emit pendingChanged();

}

// -------------------------------------------------------------------------------

CanFreezeFrame::CanFreezeFrame(QObject *parent, int dtcNr) :
    QObject(parent), m_finished(false), m_dtcNr(dtcNr)
{
}

CanFreezeFrame::~CanFreezeFrame()
{
}

void CanFreezeFrame::setPid(CanPid *pid)
{
    if (m_pids.contains(pid->key()))
        m_pids.remove(pid->key());
    m_pids.insert(pid->key(), pid);
}

void CanFreezeFrame::setFinished(bool finished)
{
    m_finished = finished;
}

bool CanFreezeFrame::isFinished() const
{
    return m_finished;
}

int CanFreezeFrame::pidCount() const
{
    return m_pids.size();
}

CanPid *CanFreezeFrame::getPid(int idx)
{
    CanPid *pid = nullptr;
    int i = 0;
    for (CanPid *p : m_pids) {
        if (i == idx) {
            pid = p;
            QQmlEngine::setObjectOwnership(p, QQmlEngine::CppOwnership);
            break;
        }
        ++i;
    }

    return pid;
}

CanPid *CanFreezeFrame::getPid(const QString &key)
{
    if (m_pids.contains(key))
        return m_pids[key];
    return nullptr;
}

int CanFreezeFrame::dtcNr() const
{
    return m_dtcNr;
}


// -------------------------------------------------------------------------------

CanAbstractNode::CanAbstractNode(can_senderIds_e canNodeId, CanInterface *canInterface, QObject *parent) :
    QObject(parent),
    m_canIface(canInterface),
    m_dtcCount(0),
    m_canNodeID(canNodeId)
{
    connect(canInterface, SIGNAL(recievedFrames(QList<QCanBusFrame>&)),
            this, SLOT(updatedFromCan(QList<QCanBusFrame>&)));
}

CanAbstractNode::~CanAbstractNode()
{
}

bool CanAbstractNode::hasProperty(const QString &key)
{
    return m_pids.keys().contains(key);
}

QList<const CanPid *> CanAbstractNode::getAllPids() const
{
    QList<const CanPid*> list;
    foreach(CanPid *pid, m_pids) {
        list.append(pid);
    }

    return list;
}

QQmlListProperty<CanPid> CanAbstractNode::getPids()
{
    return QQmlListProperty<CanPid>(this, this, &CanAbstractNode::pidCount, &CanAbstractNode::pid);
}

int CanAbstractNode::dtcCount() const
{
    return m_dtcCount;
}

int CanAbstractNode::pidCount() const
{
    return m_pids.size();
}

CanPid *CanAbstractNode::getPid(int idx) const
{
    CanPid *pid = nullptr;
    int i = 0;
    for (CanPid *p : m_pids) {
        if (i == idx) {
            pid = p;
            break;
        }
        ++i;
    }
    if (!pid)
        return nullptr;

    QQmlEngine::setObjectOwnership(pid, QQmlEngine::CppOwnership);

    return pid;
}

CanPid *CanAbstractNode::getPid(const QString key) const
{
    if (m_pids.contains(key))
        return m_pids[key];
    return nullptr;
}

CanDtc *CanAbstractNode::getDtc(int idx) const
{
    if (m_dtcs.contains(idx))
        return m_dtcs[idx];
    return nullptr;
}

int CanAbstractNode::freezeFrameCount() const
{
    return m_freezeFrames.size();
}

bool CanAbstractNode::fetchFreezeFrame(int dtcNr, QJSValue jsCallback)
{
    Q_UNUSED(dtcNr);
    Q_UNUSED(jsCallback);
    return false;
}

void CanAbstractNode::updatedFromCan(QList<QCanBusFrame> &frames)
{
    if (frames.size() > 0) {
        for (const QCanBusFrame &frame : frames) {
            quint32 sid = frame.frameId();
            if ((sid & CAN_MSG_SENDER_ID_MASK) != m_canNodeID)
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

void CanAbstractNode::setPidsValue(QString key, QString value, QString unit,
                                   PidStore &pidStore, can_senderIds_e senderId)
{
    if (!pidStore.keys().contains(key)) {
        CanPid *pid = new CanPid(key, value, unit, senderId, this);
        pidStore[key] = pid;
        emit newPid(pid);
    } else {
        pidStore[key]->setValue(value);
    }
}

void CanAbstractNode::setAmpPid(QString key, quint8 amp,
                                PidStore &pidStore, can_senderIds_e senderId)
{
    QString value = QString::number(amp);
    setPidsValue(key, value, "A", pidStore, senderId);
}

void CanAbstractNode::setVoltPid(QString key, quint16 volt,
                                 PidStore &pidStore, can_senderIds_e senderId)
{
    double fVolt = volt / 1000.0;
    QString value = QString::number(fVolt, 'f', 1);
    setPidsValue(key, value, "V", pidStore, senderId);
}

void CanAbstractNode::setBoolPid(QString key, quint8 vlu,
                                 PidStore &pidStore, can_senderIds_e senderId)
{
    QString value = vlu != 0 ? "On" : "Off";
    setPidsValue(key, value, "", pidStore, senderId);
}

void CanAbstractNode::setTempPid(QString key, quint8 temp,
                                 PidStore &pidStore, can_senderIds_e senderId)
{
    QString value = QString::number(temp);
    setPidsValue(key, value, "°C", pidStore, senderId);
}

bool CanAbstractNode::fetchDtc(int storedIdx, QJSValue jsCallback, can_msgIdsDiag_e canDiagId)
{
    if (storedIdx < 0 || storedIdx > m_dtcCount)
        return false;

    if (!jsCallback.isCallable())
        return false;

    // store callback in stack for later use
    m_dtcFetchCallback.insert(storedIdx, jsCallback);

    // already fetched
    if (m_dtcs.contains(storedIdx)) {
        dtcArriveDoQmlCallback(storedIdx);
        return true;
    }

    // get from Canbus
    QByteArray payload;
    payload.insert(0, (storedIdx & 0xFF));
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | canDiagId | C_displayNode, payload);
    return m_canIface->sendFrame(f);
}

bool CanAbstractNode::fetchAllDtcs(can_msgIdsDiag_e canDiagId)
{
    // first reset dtcCount then fetch the new dtcCount
    // when that frame arrives it will automatically retrieve these DTCs
    m_dtcCount = 0;
    // must be remote request frame
    QCanBusFrame f;//(QCanBusFrame::RemoteRequestFrame);// FlexCAN hardware limitation, can't recieve rtr
    f.setFrameId(CAN_MSG_TYPE_DIAG | canDiagId | C_displayNode);
    return m_canIface->sendFrame(f);
}

bool CanAbstractNode::clearAllDtcs(can_msgIdsDiag_e canDiagId)
{
    // gets cleared when reponse frame arrives
    QByteArray pl;
    pl.append(static_cast<quint8>(m_dtcCount & 0xFF));
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | canDiagId | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

bool CanAbstractNode::fetchFreezeFrame(int dtcNr, QJSValue jsCallback, can_msgIdsDiag_e canDiagId)
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
        // refetch
    } else {
        // we havent yet tried to fetch this frame

        // store in stack for later use
        m_freezeFrameFetchCallback.insert(dtcNr, jsCallback);
        // create a freezeFrame object for this dtc,
        // used later to check against multiple fetches
        CanFreezeFrame *ff = new CanFreezeFrame(this, dtcNr);
        m_freezeFrames.insert(dtcNr, ff);
    }

    // get from can
    QByteArray pl;
    pl.append(static_cast<quint8>(dtcNr & 0xFF));
    QCanBusFrame f(CAN_MSG_TYPE_DIAG | canDiagId | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

// callback in QML
void CanAbstractNode::dtcArriveDoQmlCallback(int dtcIdx)
{
    CanDtc *dtc = m_dtcs[dtcIdx];
    if (m_dtcFetchCallback.contains(dtcIdx)) {
        QJSValue jsCallback = m_dtcFetchCallback.take(dtcIdx);
        QJSEngine *engine = qjsEngine(this);
        if (engine) {
            QJSValue val = engine->newQObject(dtc);
            jsCallback.call(QJSValueList { val });
        }
    }
}

// callback in QML
void CanAbstractNode::freezeFrameArrival(int dtcNr)
{
    CanFreezeFrame *ff = m_freezeFrames[dtcNr];
    if (m_freezeFrameFetchCallback.contains(dtcNr)) {
        QJSValue jsCallback = m_freezeFrameFetchCallback.take(dtcNr);
        QJSEngine *engine = qjsEngine(this);
        if (engine) {
            QJSValue val = engine->newQObject(ff);
            jsCallback.call(QJSValueList { val });
        }
    }
}

bool CanAbstractNode::setSettingU8(quint8 idx, quint8 vlu, QJSValue jsCallback, can_msgIdsCommand_e canCmdId)
{

    if (!jsCallback.isCallable())
        return false;

    m_settingsSetCallback.insert(idx, jsCallback);
    QByteArray pl("\0\0", 2);
    pl[0] = idx;
    pl[1] = vlu & 0xFF;
    QCanBusFrame f(CAN_MSG_TYPE_COMMAND | canCmdId | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

bool CanAbstractNode::setSettingU16(quint8 idx, quint16 vlu, QJSValue jsCallback, can_msgIdsCommand_e canCmdId)
{

    if (!jsCallback.isCallable())
        return false;

    m_settingsSetCallback.insert(idx, jsCallback);
    QByteArray pl("\0\0\0", 3);
    pl[0] = idx;
    pl[1] = vlu & 0xFF;
    pl[2] = (vlu & 0xFF00) >> 8;
    QCanBusFrame f(CAN_MSG_TYPE_COMMAND | canCmdId | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

bool CanAbstractNode::setSettingU32(quint8 idx, quint32 vlu, QJSValue jsCallback, can_msgIdsCommand_e canCmdId)
{
    if (!jsCallback.isCallable())
        return false;

    m_settingsSetCallback.insert(idx, jsCallback);
    QByteArray pl("\0\0\0\0\0", 5);
    pl[0] = idx;
    pl[1] = (vlu & 0x000000FF);
    pl[2] = (vlu & 0x0000FF00) >> 8;
    pl[3] = (vlu & 0x00FF0000) >> 16;
    pl[4] = (vlu & 0xFF000000) >> 24;
    QCanBusFrame f(CAN_MSG_TYPE_COMMAND | canCmdId | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

bool CanAbstractNode::setSettingF(quint8 idx, float vlu, QJSValue jsCallback, can_msgIdsCommand_e canCmdId)
{
    if (!jsCallback.isCallable())
        return false;

    m_settingsSetCallback.insert(idx, jsCallback);
    quint32 *u_vlu = reinterpret_cast<quint32*>(&vlu);
    QByteArray pl("\0\0\0\0\0", 5);
    pl[0] = idx;
    pl[1] = ((*u_vlu) & 0x000000FF);
    pl[2] = ((*u_vlu) & 0x0000FF00) >> 8;
    pl[3] = ((*u_vlu) & 0x00FF0000) >> 16;
    pl[4] = ((*u_vlu) & 0xFF000000) >> 24;

    QCanBusFrame f(CAN_MSG_TYPE_COMMAND | canCmdId | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

void CanAbstractNode::settingsSetArrival(int idx, quint16 vlu)
{
    if (m_settingsSetCallback.contains(idx)) {
        QJSValue jsCallback = m_settingsSetCallback.take(idx);
        jsCallback.call(QJSValueList { vlu });
    }
}

void CanAbstractNode::settingsSetArrival32(int idx, quint32 vlu)
{
    if (m_settingsSetCallback.contains(idx)) {
        QJSValue jsCallback = m_settingsSetCallback.take(idx);
        jsCallback.call(QJSValueList { vlu });
    }
}

void CanAbstractNode::settingsFetchArrivalFloat(int idx, float vlu)
{
    if (m_settingsFetchCallback.contains(idx)) {
        QJSValue jsCallback = m_settingsFetchCallback.take(idx);
        jsCallback.call(QJSValueList { static_cast<qreal>(vlu) });
    }
}

void CanAbstractNode::settingsSetArrivalFloat(int idx, float vlu)
{
    if (m_settingsSetCallback.contains(idx)) {
        QJSValue jsCallback = m_settingsSetCallback.take(idx);
        jsCallback.call(QJSValueList { static_cast<qreal>(vlu) });
    }
}

bool CanAbstractNode::fetchSetting(quint8 idx, QJSValue jsCallback, can_msgIdsCommand_e canCmdId)
{

    if (!jsCallback.isCallable())
        return false;

    m_settingsFetchCallback.insert(idx, jsCallback);

    QByteArray pl;
    pl.append(idx & 0xFF);
    QCanBusFrame f(CAN_MSG_TYPE_COMMAND | canCmdId | C_displayNode, pl);
    return m_canIface->sendFrame(f);
}

void CanAbstractNode::settingsFetchArrival(int idx, quint16 vlu)
{
    if (m_settingsFetchCallback.contains(idx)) {
        QJSValue jsCallback = m_settingsFetchCallback.take(idx);
        jsCallback.call(QJSValueList { vlu });
    }
}

void CanAbstractNode::settingsFetchArrival32(int idx, quint32 vlu)
{
    if (m_settingsFetchCallback.contains(idx)) {
        QJSValue jsCallback = m_settingsFetchCallback.take(idx);
        jsCallback.call(QJSValueList { vlu });
    }
}


/* static */
int CanAbstractNode::pidCount(QQmlListProperty<CanPid> *list)
{
    return reinterpret_cast< CanAbstractNode* >(list->data)->pidCount();
}

/* static */
CanPid *CanAbstractNode::pid(QQmlListProperty<CanPid> *list, int idx)
{
    return reinterpret_cast< CanAbstractNode* >(list->data)->getPid(idx);
}


// --------------------------------------------------------------------------------
