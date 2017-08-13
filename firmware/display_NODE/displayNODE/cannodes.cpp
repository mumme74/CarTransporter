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
    switch ((can_DTCs_e)m_dtcCode) {
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

CanAbstractNode::CanAbstractNode(CanInterface *canInterface, QObject *parent) :
    QObject(parent),m_canIface(canInterface), m_dtcCount(0)
{
    connect(canInterface, SIGNAL(recievedFrames(QList<QCanBusFrame>&)),
            this, SLOT(updatedFromCan(QList<QCanBusFrame>&)));
}

CanAbstractNode::~CanAbstractNode()
{
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

void CanAbstractNode::setPidsValue(QString key, QString value, QString unit, PidStore &pidStore)
{
    if (!pidStore.keys().contains(key)) {
        CanPid *pid = new CanPid(key, value, unit, this);
        pidStore[key] = pid;
        emit newPid(pid);
    } else {
        pidStore[key]->setValue(value);
    }
}

void CanAbstractNode::setAmpPid(QString key, quint8 amp, PidStore &pidStore)
{
    QString value = QString::number(amp);
    setPidsValue(key, value, "A", pidStore);
}

void CanAbstractNode::setVoltPid(QString key, quint16 volt, PidStore &pidStore)
{
    double fVolt = volt / 1000;
    QString value = QString::number(fVolt, 'g', 1);
    setPidsValue(key, value, "V", pidStore);
}

void CanAbstractNode::setBoolPid(QString key, quint8 vlu, PidStore &pidStore)
{
    QString value = vlu != 0 ? "On" : "Off";
    setPidsValue(key, value, "", pidStore);
}

void CanAbstractNode::setTempPid(QString key, quint8 temp, PidStore &pidStore)
{
    QString value = QString::number(temp);
    setPidsValue(key, value, "°C", pidStore);
}

// callback in QML
void CanAbstractNode::dtcOnArrival(int dtcIdx)
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

void CanAbstractNode::settingsSetArrival(int idx, quint16 vlu)
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
        jsCallback.call(QJSValueList { vlu });
    }
}

void CanAbstractNode::settingsSetArrivalFloat(int idx, float vlu)
{
    if (m_settingsSetCallback.contains(idx)) {
        QJSValue jsCallback = m_settingsSetCallback.take(idx);
        jsCallback.call(QJSValueList { vlu });
    }
}

void CanAbstractNode::settingsFetchArrival(int idx, quint16 vlu)
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
