#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QSettings> // Added for persistent path storage
#include <QDebug>
#include <stdexcept>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {
    // Automatically attempts to open the last saved path, or defaults to Downloads
    initializeDatabase();
}

void DatabaseManager::setDatabasePath(const QString &path) {
    if (m_databasePath != path) {
        m_databasePath = path;
        emit databasePathChanged();
    }
}

bool DatabaseManager::loadExternalDatabase(const QString &filePath) {
    // Standardize path (strip QML file URL prefixes if any)
    QString cleanPath = filePath;
    if (cleanPath.startsWith("file://")) {
    #ifdef Q_OS_WIN
        cleanPath = cleanPath.mid(8); // Handles 'file:///C:/...' safely on Windows
    #else
        cleanPath = cleanPath.mid(7); // Handles 'file://...' on Linux/macOS
    #endif
    }

    qDebug() << "Attempting to shift connection to user picked DB:" << cleanPath;

    // Close down the active default connection scope cleanly before re-assigning
    {
        QSqlDatabase db = QSqlDatabase::database();
        if (db.isOpen()) {
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    // Set up connection to the newly chosen file path location
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(cleanPath);

    if (!db.open()) {
        qWarning() << "Failed to open user selected DB:" << db.lastError().text();
        //Reconnect Previous One.
        initializeDatabase();
        return false;
    }

    QString absolutePath = QFileInfo(db.databaseName()).absoluteFilePath();
    setDatabasePath(absolutePath);

    // Save this path persistently so it's remembered next time the app opens
    QSettings settings("MyMedicalAppCompany", "PatientManager");
    settings.setValue("database/lastPath", absolutePath);

    return true;
}

bool DatabaseManager::initializeDatabase()
{
    try {
        // Initialize QSettings with an Organization name and Application name
        QSettings settings("MyMedicalAppCompany", "PatientManager");

        // Read the last path. If it doesn't exist, fall back to an empty string.
        QString dbPath = settings.value("database/lastPath", "").toString();

        // Edge case: If no previous path was saved, OR if the previous file was deleted/moved
        if (dbPath.isEmpty() || !QFile::exists(dbPath)) {
            QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
            QDir dir(downloadsPath);

            if (!dir.exists() && !dir.mkpath(".")) {
                throw std::runtime_error("Failed to access Downloads directory.");
            }

            dbPath = dir.filePath("arabic_data.db");
            qDebug() << "No valid saved path found. Using default path:" << dbPath;
        } else {
            qDebug() << "Found remembered database path:" << dbPath;
        }

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);

        if (!db.open()) {
            std::string errorMsg = "Database connection failed: " + db.lastError().text().toStdString();
            throw std::runtime_error(errorMsg);
        }

        // Save the verified absolute system path to the Q_PROPERTY variable
        setDatabasePath(QFileInfo(db.databaseName()).absoluteFilePath());

        // Update settings to ensure this verified path is locked in
        settings.setValue("database/lastPath", m_databasePath);

        qDebug() << "DATABASE IS ACTIVE AT:" << m_databasePath;

        QSqlQuery query;
        QString createTableSQL =
            "CREATE TABLE IF NOT EXISTS UD1 ("
            "trav ANY, tel ANY, name ANY, age ANY, "
            "ada1 TEXT, ada2 TEXT, codea TEXT, thss TEXT, "
            "hala TEXT, datea TEXT, soibek TEXT, doia TEXT, "
            "prib1 TEXT, prib2 TEXT, rsidf TEXT, addr TEXT"
            ");";

        if (!query.exec(createTableSQL)) {
            std::string errorMsg = "Table creation failed: " + query.lastError().text().toStdString();
            throw std::runtime_error(errorMsg);
        }

        QString createud2TableSQL = R"(
        CREATE TABLE IF NOT EXISTS "UD2" (
            "CODEA"  ANY,
            "DZ1"    ANY,
            "DZ2"    ANY,
            "DZ3"    ANY,
            "DZ6"    ANY,
            "DZ7"    ANY,
            "DZ8"    ANY,
            "DZ9"    ANY,
            "DZ10"   ANY,
            "DZ11"   ANY,
            "DZ12"   ANY,
            "DZ13"   ANY,
            "DZ14"   ANY,
            "DZ15"   ANY,
            "DZ16"   ANY,
            "DZ17"   ANY,
            "DZ18"   ANY,
            "DZ19"   ANY,
            "DZ20"   ANY,
            "DZ21"   ANY,
            "DZ22"   ANY,
            "DZ23"   ANY,
            "DATEA"  ANY,
            "DZ24"   ANY,
            "codep"  ANY,
            "DZ5"    ANY,
            "DZ4"    ANY
        );
        )";

        if (!query.exec(createud2TableSQL)) {
            qDebug() << "Error creating table 'UD2':" << query.lastError().text();
            return false;
        }

        return true;

    } catch (const std::exception &e) {
        qCritical() << "CRITICAL DATABASE ERROR:" << e.what();
        return false;
    } catch (...) {
        qCritical() << "An unknown system error occurred.";
        return false;
    }
}