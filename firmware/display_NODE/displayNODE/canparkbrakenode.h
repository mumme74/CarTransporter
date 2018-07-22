#ifndef CANPARKBRAKENODE_H
#define CANPARKBRAKENODE_H

#include "cannodes.h"

class ParkbrakeConfig {
    Q_GADGET
public:
    explicit ParkbrakeConfig() {}

#include "park_settings.h"
    Q_ENUM(settings_e)
};

// ----------------------------------------------------------------

class CanParkbrakeNode : public CanAbstractNode
{
    Q_OBJECT
    Q_PROPERTY(bool serviceMode READ inServiceMode NOTIFY serviceModeChanged)
public:
    explicit CanParkbrakeNode(CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanParkbrakeNode();

    Q_INVOKABLE bool fetchDtc(int storedIdx, QJSValue jsCallback);
    Q_INVOKABLE bool fetchAllDtcs();
    Q_INVOKABLE bool clearAllDtcs();

    Q_INVOKABLE bool fetchFreezeFrame(int dtcNr, QJSValue jsCallback);

    Q_INVOKABLE bool activateOutput(int wheel, bool tighten) const;

    Q_INVOKABLE bool setServiceState(bool service);

    Q_INVOKABLE bool fetchSetting(quint8 idx, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingUInt16(quint8 idx, quint16 vlu, QJSValue jsCallback);

public slots:
    bool inServiceMode();

signals:
    void serviceModeChanged(bool service);
    void userErrorIgnOff();
    void userErrorBrakeOff();
    void userErrorBtnInvOff();
    void userError(int userError);
    void newDtcSet(CanDtc *dtc);

protected:
    void updateCanFrame(const QCanBusFrame &frame);
    void commandCanFrame(const QCanBusFrame &frame);
    void exceptionCanFrame(const QCanBusFrame &frame);
    void diagCanFrame(const QCanBusFrame &frame);

private:
    bool m_inServiceState;

    void setStatePid(QString key, quint8 state, PidStore &pidStore);
    void setWheelRevs(QString key, quint8 rev, PidStore &pidStore);
};

// ---------------------------------------------------------------

// model for parkbrakeNode DTC table
class ParkbrakeDTCModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ParkbrakeDTCModel(CanParkbrakeNode *node);
    ~ParkbrakeDTCModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    // for QML to be able to use this model
    enum Roles {
        CodeRole = Qt::UserRole + 1,
        DescRole,
        OccurencesRole,
        TimeRole
    };

private slots:
    void cleared(bool cleared, int count);
    void updated(int idx);
    void added(int idx);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    CanParkbrakeNode *m_node;
    const int m_colCount = 4;
};

#endif // CANPARKBRAKENODE_H
