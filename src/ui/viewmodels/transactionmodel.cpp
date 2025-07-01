#include "transactionmodel.h"

#include <QDateTime>
#include <QDebug>
#include "data/transactiondao.h"

namespace {
// Helpers to convert between int and enum
inline Transaction::TransactionType toEnum(int v)
{
    return v == 0 ? Transaction::TransactionType::Income
                  : Transaction::TransactionType::Expense;
}
inline int toInt(Transaction::TransactionType type)
{
    return type == Transaction::TransactionType::Income ? 0 : 1;
}
} // namespace

TransactionModel::TransactionModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadFromDatabase();
}

int TransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_transactions.size();
}

QVariant TransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_transactions.size())
        return {};

    const Transaction &tx = m_transactions.at(index.row());

    switch (role) {
    case IdRole:         return tx.id();
    case TypeRole:       return toInt(tx.type());
    case AmountRole:     return tx.amount();
    case DateRole:       return tx.date();
    case DescriptionRole:return tx.description();
    case CategoryRole:   return tx.category();
    default:             return {};
    }
}

QHash<int, QByteArray> TransactionModel::roleNames() const
{
    return {
        { IdRole, "id" },
        { TypeRole, "type" },
        { AmountRole, "amount" },
        { DateRole, "date" },
        { DescriptionRole, "description" },
        { CategoryRole, "category" }
    };
}

std::optional<int> TransactionModel::addTransaction(int type, double amount, const QDateTime &date,
                                                   const QString &description, const QString &category)
{
    Transaction tx(-1, toEnum(type), amount, date, description, category);
    auto newIdOpt = TransactionDao::addTransaction(tx);
    if (!newIdOpt)
        return std::nullopt;

    // Create a new immutable object with the generated id
    Transaction persisted(*newIdOpt, tx.type(), tx.amount(), tx.date(), tx.description(), tx.category());

    const int row = m_transactions.size();
    beginInsertRows(QModelIndex(), row, row);
    m_transactions.append(persisted);
    endInsertRows();

    return *newIdOpt;
}

bool TransactionModel::updateTransaction(int id, int type, double amount, const QDateTime &date,
                                         const QString &description, const QString &category)
{
    for (int i = 0; i < m_transactions.size(); ++i) {
        if (m_transactions[i].id() == id) {
            Transaction updated(id, toEnum(type), amount, date, description, category);
            if (!TransactionDao::updateTransaction(updated))
                return false;

            m_transactions[i] = updated;
            const QModelIndex idx = index(i);
            emit dataChanged(idx, idx);
            return true;
        }
    }
    return false;
}

bool TransactionModel::deleteTransaction(int id)
{
    for (int i = 0; i < m_transactions.size(); ++i) {
        if (m_transactions[i].id() == id) {
            if (!TransactionDao::deleteTransaction(id))
                return false;

            beginRemoveRows(QModelIndex(), i, i);
            m_transactions.removeAt(i);
            endRemoveRows();
            return true;
        }
    }
    return false;
}

void TransactionModel::refresh()
{
    beginResetModel();
    m_transactions.clear();
    loadFromDatabase();
    endResetModel();
}

void TransactionModel::loadFromDatabase()
{
    m_transactions = TransactionDao::fetchAll();
} 