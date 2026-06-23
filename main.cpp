#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<QSqlQuery>
#include <QSqlError>

#include <QStandardPaths>
#include <QDir>
#include <QSqlDatabase>
#include <QFileInfo>
#include <exception> // Required for std::exception

bool initializeDatabase() {
    try {
        QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QDir dir(downloadsPath);

        if (!dir.exists() && !dir.mkpath(".")) {
            throw std::runtime_error("Failed to access Downloads directory.");
        }

        // Change DB name to UD1.db
        QString dbPath = dir.filePath("arabic_data.db");

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);

        if (!db.open()) {
            std::string errorMsg = "Database connection failed: " + db.lastError().text().toStdString();
            throw std::runtime_error(errorMsg);
        }

        qDebug() << "DATABASE IS LOCATED AT:" << QFileInfo(db.databaseName()).absoluteFilePath();

        QSqlQuery query;
        QString createTableSQL =
            "CREATE TABLE IF NOT EXISTS UD1 ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "trav TEXT, tel TEXT, name TEXT, age TEXT, "
            "ada1 TEXT, ada2 TEXT, codea TEXT, thss TEXT, "
            "hala TEXT, datea TEXT, soibek TEXT, doia TEXT, "
            "prib1 TEXT, prib2 TEXT, rsidf TEXT, addr TEXT"
            ");";

        if (!query.exec(createTableSQL)) {
            std::string errorMsg = "Table creation failed: " + query.lastError().text().toStdString();
            throw std::runtime_error(errorMsg);
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
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // 2. CALL IT HERE: Boot up your database configuration
    if (!initializeDatabase()) {
        qCritical() << "Closing application due to database failure.";
        return -1; // Abort launch if DB cannot open
    }
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("M", "Main");



    return QGuiApplication::exec();
}
