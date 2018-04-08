#include "translation.h"

#include <QTranslator>
#include <QSettings>
#include <QGuiApplication>
#include <QDir>


Translation::Translation(QObject *parent) :
    QObject(parent)
{
    translator = new QTranslator(this);
}

Translation::~Translation()
{
    delete translator;
}

QString Translation::getEmptyString()
{
    return "";
}

void Translation::selectLanguage(QString language)
{
    if(language == QString("sv")) {
        QDir tsDir = QDir(qApp->applicationDirPath());

    #if defined(Q_OS_WIN)
        if (tsDir.dirName().toLower() == "debug" || tsDir.dirName().toLower() == "release")
            tsDir.cdUp();
    #elif defined(Q_OS_MAC)
        if (tsDir.dirName() == "MacOS") {
            tsDir.cdUp();
            tsDir.cdUp();
            tsDir.cdUp();
        }
    #endif
        tsDir.cd("ts");

        if (translator->load("ts_sv", tsDir.absolutePath()))
            qApp->installTranslator(translator);
    }

    if(language == QString("en")) {
        qApp->removeTranslator(translator);
    }

    QSettings set;
    set.setValue("language", language);

    emit languageChanged();
}

const QString Translation::getLang() const
{
    QSettings set;
    return set.value("language", "en").toString();
}
