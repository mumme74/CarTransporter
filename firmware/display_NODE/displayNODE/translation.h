#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QObject>

class QTranslator;
class Translation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString str READ getEmptyString NOTIFY languageChanged)

public:
    explicit Translation(QObject *parent = nullptr);
    ~Translation();

    QString getEmptyString();

    Q_INVOKABLE void selectLanguage(QString language);

signals:
    void languageChanged();

private:
    QTranslator *translator;
};


#endif // TRANSLATION_H
