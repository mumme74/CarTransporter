#include "canpids.h"
#include "can_protocol.h"
#include <QQmlEngine>


CanPid::CanPid() :
    QObject(),
    m_key("invalid"),
    m_value("invalid"),
    m_unit("invalid"),
    m_originNode(C_nodeInValid)
{
}

CanPid::CanPid(const QString &key, QString value, QString unit,
               can_senderIds_e senderId, QObject *parent) :
    QObject(parent),
    m_key(key),
    m_value(value),
    m_unit(unit),
    m_originNode(senderId)
{
}

CanPid::CanPid(const CanPid &other) :
    QObject(other.parent()),
    m_key(other.m_key),
    m_value(other.m_value),
    m_unit(other.m_unit),
    m_originNode(other.m_originNode)
{
}

CanPid::~CanPid()
{
}

void CanPid::setValue(QString value)
{
    m_value = value;
    emit valueChanged(this);
}

int CanPid::valueInt() const
{
    bool ok;
    int vlu = m_value.toInt(&ok);
    if (!ok)
        return 0;
    return vlu;
}

float CanPid::valueFloat() const
{
    bool ok;
    float vlu = m_value.toFloat(&ok);
    if (!ok)
        return 0.0;
    return vlu;
}

const QString CanPid::originName() const
{
    switch (m_originNode) {
    case C_parkbrakeNode:
        return tr("parkbrakeNode");
    case C_suspensionNode:
        return tr("suspensionNode");
    case C_displayNode:
        return tr("displayNode");
    default:
        return tr("unknown Node");
    }
}


// -----------------------------------------------------------------------------------------------


CanPids::CanPids(QObject *parent) :
    QAbstractTableModel(parent)
{
    qmlRegisterUncreatableType<CanPid>("mummesoft", 0, 1, "CanPid", QStringLiteral("Cant create CanPid from QML"));
    qmlRegisterUncreatableType<CanPids>("mummesoft", 0, 1, "CanPids", QStringLiteral("Cant create CanPids from QML"));
}

CanPids::~CanPids()
{
}

CanPid *CanPids::getPid(int idx) const
{
    int i = 0;
    for (CanPid *p : m_pids){
        if (i == idx)
            return p;
        ++i;
    }
    return nullptr;
}

bool CanPids::addPid(CanPid *pid)
{
    if (!pid || m_pids.contains(pid->key()))
        return false;

    // QMap sorts by key, we must insert it first in order to get index (before beginInsertRows)
    m_pids.insert(pid->key(), pid);
    int i = m_pids.keys().indexOf(pid->key());

    beginInsertRows(QModelIndex(), i, i);
    connect(pid, SIGNAL(valueChanged(const CanPid*)),
            this, SLOT(pidUpdated(const CanPid*)));
    endInsertRows();

    return true;
}

bool CanPids::removePid(CanPid *pid)
{
    if (!pid || !m_pids.contains(pid->key()))
        return false;

    int i = m_pids.keys().indexOf(pid->key());

    beginRemoveRows(QModelIndex(), i, i);
    disconnect(pid, SIGNAL(valueChanged(const CanPid*)),
               this, SLOT(pidUpdated(const CanPid*)));
    m_pids.remove(pid->key());
    endRemoveRows();
    return true;
}

int CanPids::indexOf(const CanPid *pid)
{
    return m_pids.keys().indexOf(pid->key());
}

bool CanPids::contains(const QString &key) const
{
    return m_pids.contains(key);
}

bool CanPids::contains(CanPid *pid) const
{
    return m_pids.contains(pid->key());
}

size_t CanPids::count() const
{
    return m_pids.count();
}

void CanPids::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_pids.size() -1);
    m_pids.clear();
    endRemoveRows();
}

QList<const CanPid *> CanPids::getAllPids() const
{
    QList<const CanPid*> list;
    foreach(CanPid *pid, m_pids) {
        list.append(pid);
    }

    return list;
}

QQmlListProperty<CanPid> CanPids::getPids()
{
    return QQmlListProperty<CanPid>(this, this, &CanPids::pidCount, &CanPids::pid);
}

QVariant CanPids::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((role != Qt::DisplayRole && role !=Qt::ToolTipRole) || orientation != Qt::Horizontal)
        return QVariant();

    if (section < 0 || section >= colCount)
        return QVariant();

    if (section == 0)
        return QVariant(tr("Parameter"));
    else if (section == 1)
        return QVariant(tr("Value"));
    else if (section == 2)
        return QVariant(tr("Origin"));
    return QVariant();
}

QVariant CanPids::data(const QModelIndex &index, int role) const
{
    if (index.column() < 0 || index.column() >= colCount)
        return QVariant();

    if (index.row() >= m_pids.size())
        return QVariant();

    CanPid *pid = getPid(index.row());
    if (pid == nullptr)
        return QVariant();

    if (role == KeyRole)
        return QVariant(pid->key());
    else if (role == ValueRole)
        return QVariant(pid->valueStr() + pid->unit());
    else if (role == OriginRole)
        return QVariant(pid->originName());
    else
        return QVariant();
}

int CanPids::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return colCount;
}

int CanPids::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_pids.size();
}

QHash<int, QByteArray> CanPids::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "name";
    roles[ValueRole] = "value";
    roles[OriginRole] = "origin";
    return roles;

}

void CanPids::pidUpdated(const CanPid *pid)
{
    int i = indexOf(pid);
    if (i == -1)
        return;
    QModelIndex left = createIndex(i, 0, (void*)pid);
    QModelIndex right = createIndex(i, 1, (void*)pid);
    dataChanged(left, right);
}

/* static */
int CanPids::pidCount(QQmlListProperty<CanPid> *list)
{
    return reinterpret_cast< CanPids* >(list->data)->count();
}

/* static */
CanPid *CanPids::pid(QQmlListProperty<CanPid> *list, int idx)
{
    return reinterpret_cast< CanPids* >(list->data)->getPid(idx);
}

// ------------------------------------------------------------------------
/*
QmlPidCls::QmlPidCls(QObject *parent) :
    QObject(parent)
{
}

QmlPidCls::~QmlPidCls()
{
}

const QString QmlPidCls::format() const
{
    return m_format;
}

void QmlPidCls::setFormat(const QString format)
{
    m_format = format;
    emit formatChanged();
    emit textChanged();
}

const QString QmlPidCls::pidKeyStr() const
{
    return m_pidKeyStr;
}

void QmlPidCls::setPidKeyStr(const QString pidKey)
{
    m_pidKeyStr = pidKey;
}

const QString QmlPidCls::text() const
{

    QString vlu = QString(m_format).arg()
    return
}
*/


