#ifndef CANNODES_H
#define CANNODES_H

#include "canpids.h"

#include <QObject>

class CanAbstractNode : public QObject
{
    Q_OBJECT
public:
    explicit CanAbstractNode(CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanAbstractNode();
    Q_INVOKABLE virtual bool hasProperty(const QString &key) = 0;
    Q_INVOKABLE QList<const CanPid *> getAllPids() const;


signals:
    void newPid(CanPid *pid);

protected slots:
    virtual void updatedFromCan(QList<QCanBusFrame> &frames) = 0;


protected:

    typedef QHash<QString, CanPid*> PidStore;

    void setPidsValue(QString key, QString value);
    void setAmpPid(QString key, quint8 amp);
    void setVoltPid(QString key, quint16 volt);
    void setBoolPid(QString key, quint8 vlu);
    void setTempPid(QString key, quint8 temp);

    PidStore m_pids;
    CanInterface *m_canIface;
};



class CanParkbrakeNode : public CanAbstractNode
{
    Q_OBJECT
public:
    explicit CanParkbrakeNode(CanInterface *canInterface, QObject *parent = nullptr);
    virtual ~CanParkbrakeNode();

    Q_INVOKABLE bool hasProperty(const QString &key);

protected slots:
    void updatedFromCan(QList<QCanBusFrame> &frames);

private:
    void setStatePid(QString key, quint8 state);
    void setWheelRevs(QString key, quint8 rev);


};




#endif // CANNODES_H
