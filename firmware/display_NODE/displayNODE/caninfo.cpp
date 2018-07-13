#include "caninfo.h"
#include <QQmlEngine>

CanErrorsModel::CanErrorsModel(CanInterface *interface) :
    QAbstractTableModel(interface)
{
    connect(interface, SIGNAL(errorOccurred(QString,QString)),
            this, SLOT(addError(QString,QString)));

    qmlRegisterUncreatableType<CanErrorsModel>("mummesoft", 0, 1, "CanErrorsModel", QStringLiteral("Cant create CanErrorsModel from QML"));
}

CanErrorsModel::~CanErrorsModel()
{
}

QVariant CanErrorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((role != Qt::DisplayRole && role !=Qt::ToolTipRole) || orientation != Qt::Horizontal)
        return QVariant();

    if (section < 0 || section >= colCount)
        return QVariant();

    if (section == 0)
        return QVariant(tr("Type"));
    else if (section == 1)
        return QVariant(tr("Description"));
    return QVariant();
}

QVariant CanErrorsModel::data(const QModelIndex &index, int role) const
{
    if (index.column() < 0 || index.column() >= colCount)
        return QVariant();

    if (index.row() >= m_store.size())
        return QVariant();

    if (role == TypeRole)
        return QVariant(m_store[index.row()][0]);
    else if (role == DescriptionRole)
        return QVariant(m_store[index.row()][1]);
    else
        return QVariant();

}

int CanErrorsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return colCount;
}

int CanErrorsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_store.size();
}

int CanErrorsModel::clearAll()
{
    int cnt = m_store.size();

    if (cnt > 0) {
        beginRemoveRows(QModelIndex(), 0, cnt -1);
        m_store.clear();
        endRemoveRows();
    }

    return cnt;
}

QHash<int, QByteArray> CanErrorsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    roles[DescriptionRole] = "description";
    return roles;
}

void CanErrorsModel::addError(QString errType, QString errDesc)
{
    QList<QString> err;
    err.append(errType);
    err.append(errDesc);

    beginInsertRows(QModelIndex(), 0, 0);
    m_store.push_front(err);
    endInsertRows();

    if (m_store.size() >= maxRowCount) {
        beginRemoveRows(QModelIndex(), m_store.size() -1, m_store.size() -1);
        m_store.removeLast();
        endRemoveRows();
    }
}
