#include "canpids.h"
#include "can_protocol.h"
#include <QQmlEngine>


CanPid::CanPid() :
    QObject(),
    m_key("invalid"),
    m_value("invalid"),
    m_unit("invalid")
{
}

CanPid::CanPid(const QString &key, QString value, QString unit, QObject *parent) :
    QObject(parent),
    m_key(key),
    m_value(value),
    m_unit(unit)
{
}

CanPid::CanPid(const CanPid &other) :
    QObject(other.parent()),
    m_key(other.m_key),
    m_value(other.m_value),
    m_unit(other.m_unit)
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
    if (!pid)
        return false;

    beginInsertRows(QModelIndex(), m_pids.size(), m_pids.size());
    m_pids.insertMulti(pid->key(), pid);
    connect(pid, SIGNAL(valueChanged(const CanPid*)),
            this, SLOT(pidUpdated(const CanPid*)));
    endInsertRows();

    return true;
}

bool CanPids::removePid(int idx)
{
    CanPid *pid = nullptr;
    int i = 0;
    for (CanPid *p : m_pids) {
        if (i == idx) {
            pid = p;
            break;
        }
        ++i;
    }

    if (pid == nullptr)
        return false;

    beginRemoveRows(QModelIndex(), i, i);
    disconnect(pid, SIGNAL(valueChanged(const CanPid*)),
               this, SLOT(pidUpdated(const CanPid*)));
    m_pids.remove(pid->key(), pid);
    endRemoveRows();
    return true;
}

int CanPids::indexOf(const CanPid *pid)
{
    int i = 0;
    for (CanPid *p : m_pids) {
        if (p == pid)
            return i;
        ++i;
    }
    return -1;
}

bool CanPids::contains(const QString &key) const
{
    return m_pids.contains(key);
}

bool CanPids::contains(CanPid *pid) const
{
    return m_pids.contains(pid->key(), pid);
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
