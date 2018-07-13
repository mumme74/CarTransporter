#ifndef CANINFO_H
#define CANINFO_H

#include "caninterface.h"
#include <QList>
#include <QAbstractTableModel>

/**
 * @brief The CanErrorsModel class is a tablemodel
 *        with the last 200 error msgs from CAN
 */
class CanErrorsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CanErrorsModel(CanInterface *interface);
    virtual ~CanErrorsModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;

    /// clears all items, rutns the number of items cleared
    Q_INVOKABLE int clearAll();

    // for QML to be able to use this model
    enum CanErrorTblRoles {
        TypeRole = Qt::UserRole + 1,
        DescriptionRole
    };

protected:
    QHash<int, QByteArray> roleNames() const;

private slots:
    void addError(QString errType, QString errDesc);

private:
    typedef QList< QList<QString> > CanErrorStore;
    const int colCount = 2;
    const int maxRowCount = 200;
    CanErrorStore m_store;
};

#endif // CANINFO_H
