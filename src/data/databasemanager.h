#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

/*
 * Singleton that owns the SQLite connection.
 */
class DatabaseManager {
public:
    static DatabaseManager& instance();        // global access

    QSqlDatabase& database();                  // reference to open DB
    bool isOpen() const;                       // convenience check

    ~DatabaseManager();                        // closes connection

private:
    explicit DatabaseManager(const QString& path);   // opens DB
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void initDatabase();                       // create tables

    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
