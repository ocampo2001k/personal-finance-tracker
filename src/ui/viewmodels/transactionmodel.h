#ifndef TRANSACTIONMODEL_H
#define TRANSACTIONMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <optional>

#include "core/transaction.h"

class TransactionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        AmountRole,
        DateRole,
        DescriptionRole,
        CategoryRole
    };
    Q_ENUM(Roles)

    explicit TransactionModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // CRUD operations callable from QML
    Q_INVOKABLE std::optional<int> addTransaction(int type, double amount, const QDateTime &date,
                                                 const QString &description, const QString &category);
    Q_INVOKABLE bool updateTransaction(int id, int type, double amount, const QDateTime &date,
                                       const QString &description, const QString &category);
    Q_INVOKABLE bool deleteTransaction(int id);

    Q_INVOKABLE void refresh();

private:
    void loadFromDatabase();

    QList<Transaction> m_transactions;
};

#endif // TRANSACTIONMODEL_H 