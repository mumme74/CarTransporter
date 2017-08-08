#include "translation.h"

#include <QTranslator>
#include <QSettings>
#include <QGuiApplication>


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
        translator->load("ts_sv", "ts");
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
