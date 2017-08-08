#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>


// this class is from example http://www.vladest.org/qttipsandtricks/qsettings-and-qml.html

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    ~Settings();
    Q_INVOKABLE void setValue(const QString & key, const QVariant & value);
    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

private:
    QSettings settings_;

};

#endif // SETTINGS_H
