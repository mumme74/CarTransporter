#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QSettings>
#include "caninterface.h"
#include "translation.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    // for QSettings
    QCoreApplication::setOrganizationName("mummesoft");
    QCoreApplication::setOrganizationDomain("mummesoft.com");
    QCoreApplication::setApplicationName("displayNODE");
    QSettings set;

    Translation transObj;
    QString lang = set.value("language", "en").toString();
    transObj.selectLanguage(lang);

    // start can interface
    CanInterface canIface;
    canIface.setConnected(true);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tr", &transObj);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
