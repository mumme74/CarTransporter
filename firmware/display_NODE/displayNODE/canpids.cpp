#include "canpids.h"

#include <QQmlEngine>


CanPid::CanPid(const QString &key, QObject *parent) :
    QObject(parent),
    m_key(key),
    m_value(0),
    m_state(CanPidStates::UnInitialized)
{
}

CanPid::CanPid(const QString &key, quint8 value, QObject *parent) :
    QObject(parent),
    m_key(key),
    m_value(value),
    m_state(CanPidStates::Initialized)
{
}

CanPid::~CanPid()
{
}

void CanPid::setValueFromQml(quint8 value)
{
    m_value = value;
    emit valueChangedFromQml(this);
}

void CanPid::setValue(quint8 value)
{
    m_value = value;
    emit valueChanged();
}






// -----------------------------------------------------------------------------------------------


CanPids::CanPids(CanInterface *canInterface, QObject *parent) :
    QObject(parent),
    m_canIface(canInterface)
{
    connect(m_canIface, &CanInterface::recievedFrames, this, &CanPids::updateFromCan);
    connect(m_canIface, &CanInterface::connectedChanged, this, &CanPids::canConnectSignal);

    qmlRegisterUncreatableType<CanPid>("mummesoft", 0, 1, "CanPid", QStringLiteral("Cant create CanPid from QML"));
    qmlRegisterUncreatableType<CanPids>("mummesoft", 0, 1, "canPids", QStringLiteral("Cant create CanPids from QML"));
}

CanPids::~CanPids()
{

}

bool CanPids::contains(const QString &key) const
{
    return m_pids.contains(key);
}

size_t CanPids::count() const
{
    return m_pids.count();
}

QList<const CanPid *> CanPids::getAllPids() const
{
    QList<const CanPid*> list;
    foreach(CanPid *pid, m_pids) {
        list.append(pid);
    }

    return list;
}

bool CanPids::isCanConnected() const
{
    return m_canIface->connected();
}

CanPid *CanPids::getPid(const QString &key)
{
    QHash<const QString, CanPid*>::iterator pidIt = m_pids.find(key);
    if (pidIt == m_pids.end()){
        // create as CanPidState::UnInitialized
        CanPid *pid = new CanPid(key, this);
        return *m_pids.insert(key, pid);
    }

    return *pidIt;
}

void CanPids::canConnectSignal(bool connected)
{
    emit canConnectionChanged(connected);
}


// slot invoked by the pid itself when it changes
bool CanPids::updatedFromQml(const CanPid *pid)
{
    toCan(pid->key(), pid->value());

    emit pidChangedFromQml(pid);
    emit broadcastToWeb(pid->key(), pid->value());
    return true;
}

bool CanPids::updateFromQml(const QString &key, quint8 value)
{
    bool newKey = false;
    bool updated = false;
    CanPid *pid = insert(key, value, newKey, updated);
    if (!newKey && !updated)
        return false;

    toCan(key, value);

    emit pidChangedFromQml(pid);
    emit broadcastToWeb(key, value);
    return true;
}


// recive from CAN
bool CanPids::updateFromCan(QList<QCanBusFrame> &frames)
{
    if (frames.size() > 0) {
        foreach(QCanBusFrame frame, frames) {
            quint32 id = frame.frameId();
            QByteArray data = frame.payload();

            // convert id to string we use as key in this store
            // we only use the high number bits in picaxe databuss
            char chId = (id & 0xFF0 ) >> 3;
            QChar ch(chId);
            QString key(ch);

            int idx = 0;

            // handle request such as W1=0xA8 (1 byte ID and 2 bytes data where first databyte is part of key in this class)
            if (data.size() > 1) {
                ch = data.at(0);
                key += ch;
                idx = 1;
            }

            if (!data.isEmpty()) {
                quint8 value = data.at(idx);
                bool updated = false;
                bool newCreated = false;
                CanPid *pid = insert(key, value, newCreated, updated);
                emit pidChangedFromCan(pid);
                emit broadcastToWeb(key, value);
                return true;
            }
        }
    }

    return false;
}

CanPid *CanPids::insert(const QString &key, quint8 value, bool &newCreated, bool &updated)
{

    PidStore::iterator pidIt = m_pids.find(key);
    if (pidIt != m_pids.end()) {
        // existing Pid
        newCreated = false;
        updated = (*pidIt)->value() != value;
        if (updated)
            (*pidIt)->setValue(value);

        return *pidIt;
    }

    // new pid not yet stored
    CanPid *pid = new CanPid(key, value, this);
    connect(pid, &CanPid::valueChangedFromQml, this, &CanPids::updatedFromQml);

    newCreated = true;

    return *m_pids.insert(key, pid);
}

// send to CAN
void CanPids::toCan(const QString &key, quint8 value)
{
    // construct a can frame and broadcast for sending
    QCanBusFrame frame;
    QByteArray data(1, static_cast<char>(value));
    quint32 id = key.isEmpty() ? 0 : key.at(0).toLatin1();
    id = id<< 3;


    // handle request such as W1=0xA8 (1 byte ID and 2 bytes data where first databyte is part of key in this class)
    if (key.size() > 1)
        data.prepend(key.at(1).toLatin1());

    frame.setFrameId(id);
    frame.setPayload(data);

    m_canIface->sendFrame(frame);
}

