#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<QSqlQuery>
#include <QSqlError>

#include <QStandardPaths>
#include <QDir>
#include <QSqlDatabase>
#include <QFileInfo>
#include <exception>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

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
