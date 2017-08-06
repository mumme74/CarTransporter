#ifndef CANPIDS_H
#define CANPIDS_H


#include <QObject>
#include <QHash>
#include <QVariant>
#include "caninterface.h"


/**
 * @brief The storage for each individual PID
 */
class CanPid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValueFromQml NOTIFY valueChanged)
public:
    CanPid(const QString &key, QObject *parent);
    CanPid(const QString &key, quint8 value,QObject *parent);
    ~CanPid();

    enum CanPidStates : quint8 {
        UnInitialized = 0,
        Initialized = 1
    };

    Q_INVOKABLE bool isInitialized() { return m_state != CanPidStates::UnInitialized; }

    void setValueFromQml(quint8 value);
    void setValue(quint8 value);
    quint8 value() const { return m_value; }
    Q_INVOKABLE const QString key() const { return m_key; }


signals:
    void valueChanged();
    void valueChangedFromQml(const CanPid *pid);

private:
    const QString m_key;
    quint8 m_value;
    CanPidStates m_state;
};



/**
 * @brief A in memory database of the current states on the network
 */
class CanPids : public QObject
{
    Q_OBJECT
public:
    explicit CanPids(CanInterface *canInterface, QObject *parent = 0);
    ~CanPids();

    // returns connect state of can interface
    bool isCanConnected() const;

    // get pointer to the can pid
    Q_INVOKABLE CanPid *getPid(const QString &key);

    Q_INVOKABLE bool contains(const QString &key) const;
    Q_INVOKABLE size_t count() const;

    QList<const CanPid *> getAllPids() const;


signals:
    void pidChangedFromCan(const CanPid *pid);
    void pidChangedFromQml(const CanPid *pid);
    void pidChangedFromWeb(const CanPid *pid);

    // update when changes occur from Can and Web
    // intended for Qml slots
    void broadcastToQml(const QString &key, quint8 value);

    // update when changes occur from Can and Qqml
    // intended for websock server
    void broadcastToWeb(const QString &key, quint8 value);

    void canConnectionChanged(bool connected);

public slots:
    bool updateFromCan(QList<QCanBusFrame> &frames);
    bool updateFromQml(const QString &key, quint8 value);

    // called by the Pid itself, value already changed
    bool updatedFromQml(const CanPid *pid);


private slots:
    void canConnectSignal(bool connected);


private:
    typedef QHash<const QString, CanPid*> PidStore;
    CanPid *insert(const QString &key, quint8 value, bool &newCreated, bool &updated);
    void toCan(const QString &key, quint8 value);

    PidStore m_pids;
    CanInterface *m_canIface;
};

#endif // CANPIDS_H
