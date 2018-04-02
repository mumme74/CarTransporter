#ifndef CANNODES_H
#define CANNODES_H

#include "canpids.h"
#include "can_protocol.h"

#include <QObject>
#include <QQmlEngine>

class CanDtc : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int storedNr READ storedNr)
    Q_PROPERTY(int dtcCode READ dtcCode)
    Q_PROPERTY(QString dtcCodeStr READ dtcCodeStr)
    Q_PROPERTY(int occurences READ occurences NOTIFY occurencesChanged)
    Q_PROPERTY(int timeSinceStartup READ timeSinceStartup)
    Q_PROPERTY(QString dtcDescription READ dtcDescription)
    Q_PROPERTY(bool isPending READ isPending NOTIFY pendingChanged)

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
    void setOccurences(int occurences);
    int timeSinceStartup() const;
    bool isPending() const;
    void setPending(bool pending);

signals:
    void occurencesChanged();
    void pendingChanged();

private:
    int m_storedNr;
    int m_dtcCode;
    int m_occurences;
    int m_timeSinceStartup;
    bool m_pending;
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
    explicit CanAbstractNode(can_senderIds_e canNodeId, CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanAbstractNode();
    Q_INVOKABLE virtual bool hasProperty(const QString &key);
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
    Q_INVOKABLE virtual bool fetchAllDtcs() = 0;
    Q_INVOKABLE virtual bool clearAllDtcs() = 0;

    // FreezeFrames
    Q_INVOKABLE int freezeFrameCount() const;
    Q_INVOKABLE virtual bool fetchFreezeFrame(int dtcNr, QJSValue jsCallback);


signals:
    void newPid(CanPid *pid);
    void dtcCountChanged(int newCount);
    void dtcArrived(int storedIndex);
    void dtcsCleared(bool success);



protected:

    typedef QMap<QString, CanPid*> PidStore;

    void setPidsValue(QString key, QString value, QString unit,
                      PidStore &pidStore, can_senderIds_e senderId);
    void setAmpPid(QString key, quint8 amp, PidStore &pidStore, can_senderIds_e senderId);
    void setVoltPid(QString key, quint16 volt, PidStore &pidStore, can_senderIds_e senderId);
    void setBoolPid(QString key, quint8 vlu, PidStore &pidStore, can_senderIds_e senderId);
    void setTempPid(QString key, quint8 temp, PidStore &pidStore, can_senderIds_e senderId);


    bool fetchDtc(int storedIdx, QJSValue jsCallback, can_msgIdsDiag_e canDiagId);
    bool fetchAllDtcs(can_msgIdsDiag_e canDiagId);
    bool clearAllDtcs(can_msgIdsDiag_e canDiagId);
    void dtcOnArrival(int dtcIdx);

    bool fetchFreezeFrame(int dtcNr, QJSValue jsCallback, can_msgIdsDiag_e canDiagId);
    void freezeFrameArrival(int dtcNr);
    bool setSettingU16(quint8 idx, quint16 vlu, QJSValue jsCallback, can_msgIdsCommand_e canCmdId);
    bool setSettingU32(quint8 idx, quint32 vlu, QJSValue jsCallback, can_msgIdsCommand_e canCmdId);
    bool setSettingF(quint8 idx, float vlu, QJSValue jsCallback, can_msgIdsCommand_e canCmdId);
    bool fetchSetting(quint8 idx, QJSValue jsCallback, can_msgIdsCommand_e canCmdId);

    void settingsFetchArrival(int idx, quint16 vlu);
    void settingsSetArrival(int idx, quint16 vlu);
    void settingsFetchArrivalFloat(int idx, float vlu);
    void settingsSetArrivalFloat(int idx, float vlu);

    // callbacks when we have recived from CAN
    virtual void updateCanFrame(const QCanBusFrame &frame) = 0;
    virtual void commandCanFrame(const QCanBusFrame &frame) = 0;
    virtual void exceptionCanFrame(const QCanBusFrame &frame) = 0;
    virtual void diagCanFrame(const QCanBusFrame &frame) = 0;

    // this is a stack for callbacks to invoke when DTC arrives from CAN bus
    QMap<int, QJSValue> m_dtcFetchCallback;
    QMap<int, QJSValue> m_freezeFrameFetchCallback;
    QMap<int, QJSValue> m_settingsFetchCallback;
    QMap<int, QJSValue> m_settingsSetCallback;


    PidStore m_pids;
    CanInterface *m_canIface;
    int m_dtcCount;
    QMap<int, CanDtc*> m_dtcs;
    QMap<int, CanFreezeFrame*> m_freezeFrames;

protected slots:
    void updatedFromCan(QList<QCanBusFrame> &frames);

private:
    can_senderIds_e m_canNodeID;
    static int pidCount(QQmlListProperty<CanPid>* list);
    static CanPid* pid(QQmlListProperty<CanPid>* list, int idx);
};

// ----------------------------------------------------------------------------------




#endif // CANNODES_H
