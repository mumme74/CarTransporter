#ifndef CANNODES_H
#define CANNODES_H

#include "canpids.h"

#include <QObject>
#include <QQmlEngine>

class CanDtc : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int storedNr READ storedNr)
    Q_PROPERTY(int dtcCode READ dtcCode)
    Q_PROPERTY(QString dtcCodeStr READ dtcCodeStr)
    Q_PROPERTY(int occurences READ occurences)
    Q_PROPERTY(int timeSinceStartup READ timeSinceStartup)
    Q_PROPERTY(QString dtcDescription READ dtcDescription)

public:
    CanDtc();
    CanDtc(const CanDtc &other);
    CanDtc(QObject *parent, int storedNr, int dtcCode,
           int occurences, int timeSinceStartup);
    ~CanDtc();
    int storedNr() const;
    int dtcCode() const;
    const QString dtcCodeStr() const;
    const QString dtcDescription() const;
    int occurences() const;
    int timeSinceStartup() const;

private:
    int m_storedNr;
    int m_dtcCode;
    int m_occurences;
    int m_timeSinceStartup;
};

Q_DECLARE_METATYPE(CanDtc)


// ----------------------------------------------------------------------------------

class CanFreezeFrame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pidCount READ pidCount)
    Q_PROPERTY(bool finished READ isFinished)
public:
    CanFreezeFrame(QObject *parent, int dtcNr);
    ~CanFreezeFrame();
    bool isFinished() const;
    int pidCount() const;

    Q_INVOKABLE CanPid *getPid(int idx);
    Q_INVOKABLE CanPid *getPid(const QString &key);
    Q_INVOKABLE int dtcNr() const;



private:
    void setFinished(bool finished);
    void setPid(CanPid *pid);

    bool m_finished;
    int m_dtcNr;
    QMap<QString, CanPid*> m_pids;

    friend class CanAbstractNode;
    friend class CanParkbrakeNode;

};

// --------------------------------------------------------------------------

class CanAbstractNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<CanPid> pids READ getPids NOTIFY newPid)
    Q_PROPERTY(int dtcCount READ dtcCount NOTIFY dtcCountChanged)

public:
    explicit CanAbstractNode(CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanAbstractNode();
    Q_INVOKABLE virtual bool hasProperty(const QString &key) = 0;
    Q_INVOKABLE QList<const CanPid *> getAllPids() const;

    // PIDs
    int pidCount() const;
    Q_INVOKABLE CanPid *getPid(int idx) const;
    Q_INVOKABLE CanPid *getPid(const QString key) const;
    QQmlListProperty<CanPid> getPids();

    // DTCs
    int dtcCount() const;
    Q_INVOKABLE CanDtc *getDtc(int idx) const;
    Q_INVOKABLE virtual bool fetchDtc(int storedIdx, QJSValue jsCallback) = 0;
    Q_INVOKABLE virtual void fetchAllDtcs() = 0;
    Q_INVOKABLE virtual void clearAllDtcs() = 0;

    // FreezeFrames
    Q_INVOKABLE int freezeFrameCount() const;
    Q_INVOKABLE virtual bool fetchFreezeFrame(int dtcNr, QJSValue jsCallback) = 0;


signals:
    void newPid(CanPid *pid);
    void dtcCountChanged(int newCount);
    void dtcArrived(int storedIndex);
    void dtcsCleared(bool success);

protected slots:
    virtual void updatedFromCan(QList<QCanBusFrame> &frames) = 0;


protected:

    typedef QMap<QString, CanPid*> PidStore;

    void setPidsValue(QString key, QString value, QString unit, PidStore &pidStore);
    void setAmpPid(QString key, quint8 amp, PidStore &pidStore);
    void setVoltPid(QString key, quint16 volt, PidStore &pidStore);
    void setBoolPid(QString key, quint8 vlu, PidStore &pidStore);
    void setTempPid(QString key, quint8 temp, PidStore &pidStore);

    void dtcOnArrival(int dtcIdx);

    void freezeFrameArrival(int dtcNr);

    // this is a stack for callbacks to invoke when DTC arrives from CAN bus
    QMap<int, QJSValue> m_dtcFetchCallback;
    QMap<int, QJSValue> m_freezeFrameFetchCallback;


    PidStore m_pids;
    CanInterface *m_canIface;
    int m_dtcCount;
    QMap<int, CanDtc*> m_dtcs;
    QMap<int, CanFreezeFrame*> m_freezeFrames;

private:
    static int pidCount(QQmlListProperty<CanPid>* list);
    static CanPid* pid(QQmlListProperty<CanPid>* list, int idx);
};

// ----------------------------------------------------------------------------------

class CanParkbrakeNode : public CanAbstractNode
{
    Q_OBJECT
public:
    explicit CanParkbrakeNode(CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanParkbrakeNode();

    Q_INVOKABLE bool hasProperty(const QString &key);

    Q_INVOKABLE bool fetchDtc(int storedIdx, QJSValue jsCallback);
    Q_INVOKABLE void fetchAllDtcs();
    Q_INVOKABLE void clearAllDtcs();

    Q_INVOKABLE virtual bool fetchFreezeFrame(int dtcNr, QJSValue jsCallback);

protected slots:
    void updatedFromCan(QList<QCanBusFrame> &frames);

private:

    void updateCanFrame(const QCanBusFrame &frame);
    void commandCanFrame(const QCanBusFrame &frame);
    void exceptionCanFrame(const QCanBusFrame &frame);
    void diagCanFrame(const QCanBusFrame &frame);

    void setStatePid(QString key, quint8 state, PidStore &pidStore);
    void setWheelRevs(QString key, quint8 rev, PidStore &pidStore);
};




#endif // CANNODES_H
