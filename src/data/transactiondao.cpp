#include "transactiondao.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "databasemanager.h"
#include "core/transaction.h"

// Helper to map enum <-> int
static int toInt(Transaction::TransactionType type)
{
    return type == Transaction::TransactionType::Income ? 0 : 1;
}

static Transaction::TransactionType toEnum(int v)
{
    return v == 0 ? Transaction::TransactionType::Income
                  : Transaction::TransactionType::Expense;
}

std::optional<int> TransactionDao::addTransaction(const Transaction &transaction)
{
    QSqlDatabase &db = DatabaseManager::instance().database();
    QSqlQuery q(db);
    q.prepare("INSERT INTO transactions (type, amount, date, description, category) "
              "VALUES (:type, :amount, :date, :description, :category)");
    q.bindValue(":type", toInt(transaction.type()));
    q.bindValue(":amount", transaction.amount());
    q.bindValue(":date", transaction.date().toString(Qt::ISODate));
    q.bindValue(":description", transaction.description());
    q.bindValue(":category", transaction.category());

    if (!q.exec()) {
        qWarning() << "addTransaction error:" << q.lastError().text();
        return std::nullopt;
    }

    // Retrieve generated primary key
    bool ok = false;
    int generatedId = q.lastInsertId().toInt(&ok);
    if (!ok) {
        qWarning() << "Unable to convert lastInsertId to int";
        return std::nullopt;
    }
    return generatedId;
}

bool TransactionDao::updateTransaction(const Transaction &transaction)
{
    QSqlDatabase &db = DatabaseManager::instance().database();
    QSqlQuery q(db);
    q.prepare("UPDATE transactions SET type=:type, amount=:amount, date=:date, "
              "description=:description, category=:category WHERE id=:id");
    q.bindValue(":type", toInt(transaction.type()));
    q.bindValue(":amount", transaction.amount());
    q.bindValue(":date", transaction.date().toString(Qt::ISODate));
    q.bindValue(":description", transaction.description());
    q.bindValue(":category", transaction.category());
    q.bindValue(":id", transaction.id());

    if (!q.exec()) {
        qWarning() << "updateTransaction error:" << q.lastError().text();
        return false;
    }
    return true;
}

bool TransactionDao::deleteTransaction(int id)
{
    QSqlDatabase &db = DatabaseManager::instance().database();
    QSqlQuery q(db);
    q.prepare("DELETE FROM transactions WHERE id=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "deleteTransaction error:" << q.lastError().text();
        return false;
    }
    return true;
}

QList<Transaction> TransactionDao::fetchAll()
{
    QList<Transaction> list;
    QSqlDatabase &db = DatabaseManager::instance().database();
    QSqlQuery q(db);
    if (!q.exec("SELECT id, type, amount, date, description, category FROM transactions ORDER BY date DESC")) {
        qWarning() << "fetchAll error:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        int id = q.value(0).toInt();
        int typeInt = q.value(1).toInt();
        double amount = q.value(2).toDouble();
        QDateTime date = QDateTime::fromString(q.value(3).toString(), Qt::ISODate);
        QString description = q.value(4).toString();
        QString category = q.value(5).toString();

        list.emplace_back(id, toEnum(typeInt), amount, date, description, category);
    }
    return list;
} 