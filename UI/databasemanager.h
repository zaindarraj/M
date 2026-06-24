#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QQmlEngine>

class DatabaseManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // Updated property with a WRITE function so QML can change it
    Q_PROPERTY(QString databasePath READ databasePath WRITE setDatabasePath NOTIFY databasePathChanged)

public:
    explicit DatabaseManager(QObject *parent = nullptr);

    Q_INVOKABLE bool initializeDatabase();

    // New method to load a specific path chosen by the user
    Q_INVOKABLE bool loadExternalDatabase(const QString &filePath);

    QString databasePath() const { return m_databasePath; }
    void setDatabasePath(const QString &path);

signals:
    void databasePathChanged();

private:
    QString m_databasePath;
};

#endif // DATABASEMANAGER_H