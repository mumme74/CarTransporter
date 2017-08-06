#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "caninterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);


    // for QSettings
    QCoreApplication::setOrganizationName("mummesoft");
    QCoreApplication::setOrganizationDomain("mummesoft.com");
    QCoreApplication::setApplicationName("displayNODE");

    // start can interface
    CanInterface canIface;
    canIface.setConnected(true);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
