#ifndef CANPARKBRAKENODE_H
#define CANPARKBRAKENODE_H

#include "cannodes.h"

class CanParkbrakeNode : public CanAbstractNode
{
    Q_OBJECT
    Q_PROPERTY(bool serviceMode READ inServiceMode NOTIFY serviceModeChanged)
public:
    explicit CanParkbrakeNode(CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanParkbrakeNode();

    Q_INVOKABLE bool hasProperty(const QString &key);

    Q_INVOKABLE bool fetchDtc(int storedIdx, QJSValue jsCallback);
    Q_INVOKABLE void fetchAllDtcs();
    Q_INVOKABLE void clearAllDtcs();

    Q_INVOKABLE bool fetchFreezeFrame(int dtcNr, QJSValue jsCallback);

    Q_INVOKABLE bool activateOutput(int wheel, bool tighten) const;

    Q_INVOKABLE void setServiceState(bool service);

    Q_INVOKABLE bool fetchSetting(quint8 idx, QJSValue jsCallback);
    Q_INVOKABLE bool setSetting(quint8 idx, quint16 vlu, QJSValue jsCallback);

public slots:
    bool inServiceMode();

signals:
    void serviceModeChanged(bool service);
    void userErrorIgnOff();
    void userErrorBrakeOff();
    void userErrorBtnInvOff();
    void userError(int userError);
    void newDtcSet(CanDtc *dtc);

protected slots:
    void updatedFromCan(QList<QCanBusFrame> &frames);

private:
    bool m_inServiceState;
    void updateCanFrame(const QCanBusFrame &frame);
    void commandCanFrame(const QCanBusFrame &frame);
    void exceptionCanFrame(const QCanBusFrame &frame);
    void diagCanFrame(const QCanBusFrame &frame);

    void setStatePid(QString key, quint8 state, PidStore &pidStore);
    void setWheelRevs(QString key, quint8 rev, PidStore &pidStore);
};

#endif // CANPARKBRAKENODE_H
