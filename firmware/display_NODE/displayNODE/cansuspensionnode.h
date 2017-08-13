#ifndef CANSUSPENSIONNODE_H
#define CANSUSPENSIONNODE_H

#include "cannodes.h"


class CanSuspensionNode : public CanAbstractNode
{
    Q_OBJECT
public:
    explicit CanSuspensionNode(CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanSuspensionNode();

    Q_INVOKABLE bool hasProperty(const QString &key);

    Q_INVOKABLE bool fetchDtc(int storedIdx, QJSValue jsCallback);
    Q_INVOKABLE void fetchAllDtcs();
    Q_INVOKABLE void clearAllDtcs();

    Q_INVOKABLE bool activateOutput(int wheel, bool tighten) const;

    Q_INVOKABLE bool fetchSetting(quint8 idx, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingUint16(quint8 idx, quint16 vlu, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingUint32(quint8 idx, quint32 vlu, QJSValue jsCallback);
    Q_INVOKABLE bool setSettingFloat(quint8 idx, float vlu, QJSValue jsCallback);

signals:
    void userError(int userError);
    void newDtcSet(CanDtc *dtc);

protected slots:
    void updatedFromCan(QList<QCanBusFrame> &frames);

private:
    void updateCanFrame(const QCanBusFrame &frame);
    void commandCanFrame(const QCanBusFrame &frame);
    void exceptionCanFrame(const QCanBusFrame &frame);
    void diagCanFrame(const QCanBusFrame &frame);

private:

};

#endif // CANSUSPENSIONNODE_H
