#ifndef TRANSACTIONDAO_H
#define TRANSACTIONDAO_H

#include <QList>
#include <optional>

class Transaction;

class TransactionDao {
public:
    // Inserts and returns generated id (std::nullopt on failure)
    static std::optional<int> addTransaction(const Transaction &transaction);

    static bool updateTransaction(const Transaction &transaction);

    static bool deleteTransaction(int id);

    static QList<Transaction> fetchAll();
};

#endif // TRANSACTIONDAO_H 