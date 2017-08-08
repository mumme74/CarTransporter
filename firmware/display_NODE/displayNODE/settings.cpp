#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

Settings::~Settings()
{
}

void Settings::setValue(const QString &key, const QVariant &value) {
    settings_.setValue(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const {
    return settings_.value(key, defaultValue);
}
