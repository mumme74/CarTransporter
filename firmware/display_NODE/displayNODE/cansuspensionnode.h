#ifndef CANSUSPENSIONNODE_H
#define CANSUSPENSIONNODE_H

#include "cannodes.h"
#include "PID.h"


class CanSuspensionNode : public CanAbstractNode
{
    Q_OBJECT
public:
    explicit CanSuspensionNode(CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanSuspensionNode();

    Q_INVOKABLE bool fetchDtc(int storedIdx, QJSValue jsCallback);
    Q_INVOKABLE void fetchAllDtcs();
    Q_INVOKABLE void clearAllDtcs();

    Q_INVOKABLE bool activateOutput(quint8 pid, quint8 vlu) const;

    Q_INVOKABLE bool fetchSetting(quint8 idx, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingUint16(quint8 idx, quint16 vlu, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingUint32(quint8 idx, quint32 vlu, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingFloat(quint8 idx, float vlu, QJSValue jsCallback);

signals:
    void userError(int userError);
    void userErrorHeightNonValidState();
    void userErrorSuckingNotAllowed();
    void newDtcSet(CanDtc *dtc);

protected:
    void updateCanFrame(const QCanBusFrame &frame);
    void commandCanFrame(const QCanBusFrame &frame);
    void exceptionCanFrame(const QCanBusFrame &frame);
    void diagCanFrame(const QCanBusFrame &frame);

private:
    void setAirFeedStatePid(QString key, quint16 state, PidStore &pidStore);
    void setHeightStatePid(QString key, quint16 state, PidStore &pidStore);

};

#endif // CANSUSPENSIONNODE_H
