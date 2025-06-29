#include \"databasemanager.h\"

#include <QStandardPaths>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

// ---------- singleton access ----------
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager _instance(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
        \"/finance.db\");
    return _instance;
}

// ---------- constructor / destructor ----------
DatabaseManager::DatabaseManager(const QString& path)
{
    // Ensure directory for database file exists
    QFileInfo dbInfo(path);
    QDir dir = dbInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "Failed to create directory for DB:" << dir.absolutePath();
        }
    }

    m_database = QSqlDatabase::addDatabase(\"QSQLITE\");
    m_database.setDatabaseName(path);

    if (!m_database.open()) {
        qWarning() << \"Failed to open DB:\" << m_database.lastError().text();
    } else {
        initDatabase();
    }
}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) {
        const QString connectionName = m_database.connectionName();
        m_database.close();
        m_database = QSqlDatabase(); // release reference before removal
        QSqlDatabase::removeDatabase(connectionName);
    }
}

// ---------- public helpers ----------
QSqlDatabase& DatabaseManager::database()
{
    return m_database;
}

bool DatabaseManager::isOpen() const
{
    return m_database.isOpen();
}

// ---------- private ----------
void DatabaseManager::initDatabase()
{
    // basic Transactions table
    const QString createSql =
        \"CREATE TABLE IF NOT EXISTS transactions (\"      \
        \"id INTEGER PRIMARY KEY AUTOINCREMENT,\"           \
        \"type INTEGER NOT NULL,\"                          \
        \"amount REAL NOT NULL,\"                           \
        \"date TEXT NOT NULL,\"                             \
        \"description TEXT,\"                               \
        \"category TEXT);\";

    QSqlQuery q(m_database);
    if (!q.exec(createSql)) {
        qWarning() << \"DB init error:\" << q.lastError().text();
    }
}
