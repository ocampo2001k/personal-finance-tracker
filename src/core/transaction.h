#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDateTime>
#include <QString>

class Transaction {
public:
    enum class TransactionType {
        Income,
        Expense
    };

    Transaction(int id, TransactionType type, double amount, const QDateTime& date, const QString& description, const QString& category);

    // Getters
    int id() const;
    TransactionType type() const;
    double amount() const;
    const QDateTime& date() const;
    const QString& description() const;
    const QString& category() const;

    // Setters
    void setType(TransactionType type);
    void setAmount(double amount);
    void setDate(const QDateTime& date);
    void setDescription(const QString& description);
    void setCategory(const QString& category);

private:
    int m_id;
    TransactionType m_type;
    double m_amount;
    QDateTime m_date;
    QString m_description;
    QString m_category;
};

#endif // TRANSACTION_H
