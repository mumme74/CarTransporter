#ifndef CANSUSPENSIONNODE_H
#define CANSUSPENSIONNODE_H

#include "cannodes.h"
#include "PID.h"

class SuspensionConfig {
    Q_GADGET
public:
    explicit SuspensionConfig() {}

#include "suspension_config.h"
    Q_ENUM(Configs)
};



// ----------------------------------------------------------------

class CanSuspensionNode : public CanAbstractNode
{
    Q_OBJECT
public:
    explicit CanSuspensionNode(CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanSuspensionNode();

    Q_INVOKABLE bool fetchDtc(int storedIdx, QJSValue jsCallback);
    Q_INVOKABLE bool fetchAllDtcs();
    Q_INVOKABLE bool clearAllDtcs();

    Q_INVOKABLE bool activateOutput(quint8 pid, quint8 vlu) const;
    Q_INVOKABLE bool clearActivateOutput(quint8 pid) const;

    Q_INVOKABLE bool fetchSetting(quint8 idx, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingUInt8(quint8 idx, quint8 vlu, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingUInt16(quint8 idx, quint16 vlu, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingUInt32(quint8 idx, quint32 vlu, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingFloat(quint8 idx, float vlu, QJSValue jsCallback);

    Q_INVOKABLE bool setHeightState(const QString &state, QJSValue jsCallback);

signals:
    void userError(int userError);
    void userErrorHeightNonValidState();
    void userErrorSuckingNotAllowed();
    void newDtcSet(CanDtc *dtc);
    void activateOutputComfirmed(quint8 pid, quint8 value);
    void clearActivateOutputComfirmed(quint8 pid);
    void heightStateChanged(QString state);

protected:
    void updateCanFrame(const QCanBusFrame &frame);
    void commandCanFrame(const QCanBusFrame &frame);
    void exceptionCanFrame(const QCanBusFrame &frame);
    void diagCanFrame(const QCanBusFrame &frame);

private:
    void setAirFeedStatePid(QString key, quint16 state, PidStore &pidStore);
    void setHeightStatePid(QString key, quint16 state, PidStore &pidStore);

    QMap<int, QJSValue> m_heightStateSetCallback;
};

// ---------------------------------------------------------------

// model for systempressure dialog
class SuspensionPressureDlgModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SuspensionPressureDlgModel(CanSuspensionNode *node,
                                        QObject *parent = 0);
    ~SuspensionPressureDlgModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    // for QML to be able to use this model
    enum Roles {
        KeyRole = Qt::UserRole + 1,
        ValueRole
    };

private slots:
    void pidUpdated(const CanPid *pid);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    CanPid *getPidForIdx(int idx) const;
    CanSuspensionNode *m_node;
    const int colCount = 2;
    int m_rowCount;
};

// ---------------------------------------------------------------

// model for suspensionNode DTC table
class SuspensionDTCModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SuspensionDTCModel(CanSuspensionNode *node);
    ~SuspensionDTCModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    // for QML to be able to use this model
    enum Roles {
        CodeRole = Qt::UserRole + 1,
        DescRole,
        OccurencesRole,
        PendingRole
    };

private slots:
    void cleared(bool cleared, int count);
    void updated(int idx);
    void added(int idx);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    CanSuspensionNode *m_node;
    const int m_colCount = 4;
};


#endif // CANSUSPENSIONNODE_H
