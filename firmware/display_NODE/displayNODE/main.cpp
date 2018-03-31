#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QSettings>
#include "caninterface.h"
#include "translation.h"
#include "settings.h"
#include "cannodes.h"
#include "canparkbrakenode.h"
#include "cansuspensionnode.h"


//QQmlApplicationEngine *qmlEngine = nullptr;

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

    CanParkbrakeNode parkbrakeNode(&canIface);
    CanSuspensionNode suspensionNode(&canIface);
    CanPids canPidsModel;

    Settings settings;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tr", &transObj);
    engine.rootContext()->setContextProperty("parkbrakeNode", &parkbrakeNode);
    engine.rootContext()->setContextProperty("suspensionNode", &suspensionNode);
    engine.rootContext()->setContextProperty("canPidsModel", &canPidsModel);
    engine.rootContext()->setContextProperty("settings", &settings);


    qmlRegisterInterface<CanPid>("CanDtc");
    qmlRegisterInterface<CanPid>("CanPid");
    qmlRegisterInterface<CanPid>("CanFreezeFrame");


    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
