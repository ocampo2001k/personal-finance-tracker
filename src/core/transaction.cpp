#include "transaction.h"

Transaction::Transaction(int id, TransactionType type, double amount, const QDateTime& date, const QString& description, const QString& category)
    : m_id(id),
      m_type(type),
      m_amount(amount),
      m_date(date),
      m_description(description),
      m_category(category)
{
}

int Transaction::id() const
{
    return m_id;
}

Transaction::TransactionType Transaction::type() const
{
    return m_type;
}

double Transaction::amount() const
{
    return m_amount;
}

const QDateTime& Transaction::date() const
{
    return m_date;
}

const QString& Transaction::description() const
{
    return m_description;
}

const QString& Transaction::category() const
{
    return m_category;
}

void Transaction::setType(TransactionType type)
{
    m_type = type;
}

void Transaction::setAmount(double amount)
{
    m_amount = amount;
}

void Transaction::setDate(const QDateTime& date)
{
    m_date = date;
}

void Transaction::setDescription(const QString& description)
{
    m_description = description;
}

void Transaction::setCategory(const QString& category)
{
    m_category = category;
}
