#ifndef CANPIDS_H
#define CANPIDS_H


#include <QObject>
#include <QMultiMap>
#include <QVariant>
#include <QAbstractTableModel>
#include <QQmlListProperty>
#include "caninterface.h"
#include "can_protocol.h"


/**
 * @brief The storage for each individual PID
 */
class CanPid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString valueStr READ valueStr NOTIFY valueChanged)
    Q_PROPERTY(int valueInt READ valueInt NOTIFY valueChanged)
    Q_PROPERTY(float valueFloat READ valueFloat NOTIFY valueChanged)
    Q_PROPERTY(QString key READ key)
    Q_PROPERTY(QString unit READ unit)
    Q_PROPERTY(QString originName READ originName)
    Q_PROPERTY(can_senderIds_e origin READ origin)

public:
    explicit CanPid();
    explicit CanPid(const QString &key, QString value, QString unit,
                    can_senderIds_e senderId, QObject *parent);
    explicit CanPid(const CanPid &other);
    ~CanPid();

    void setValue(QString value);
    QString valueStr() const { return m_value; }
    int valueInt() const;
    float valueFloat() const;
    const QString key() const { return m_key; }
    const QString unit() const { return m_unit; }
    const QString originName() const;
    can_senderIds_e origin() const { return m_originNode; }



signals:
    void valueChanged(const CanPid *pid);

private:
    const QString m_key;
    QString  m_value;
    QString m_unit;
    can_senderIds_e m_originNode;
};

Q_DECLARE_METATYPE(CanPid)


// model which QMl connects to
class CanPids : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<CanPid> pids READ getPids)
public:
    explicit CanPids(QObject *parent = 0);
    ~CanPids();

    // get pointer to the can pid
    Q_INVOKABLE CanPid *getPid(int idx) const;
    Q_INVOKABLE bool addPid(CanPid *pid);
    Q_INVOKABLE bool removePid(CanPid *pid);

    Q_INVOKABLE int indexOf(const CanPid *pid);
    Q_INVOKABLE bool contains(const QString &key) const;
    Q_INVOKABLE bool contains(CanPid *pid) const;
    Q_INVOKABLE size_t count() const;
    Q_INVOKABLE void clear();

    QList<const CanPid *> getAllPids() const;

    QQmlListProperty<CanPid> getPids();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;

    // for QML to be able to use this model
    enum CanPidsRoles {
        KeyRole = Qt::UserRole + 1,
        ValueRole,
        OriginRole
    };


protected:
    QHash<int, QByteArray> roleNames() const;

private slots:
    void pidUpdated(const CanPid *pid);

private:
    static int pidCount(QQmlListProperty<CanPid>* list);
    static CanPid* pid(QQmlListProperty<CanPid>* list, int idx);


    typedef QMap<QString, CanPid*> PidStore;
    const int colCount = 3;
    PidStore m_pids;
};

#endif // CANPIDS_H
