#ifndef KEYBOARDEVENTS_H
#define KEYBOARDEVENTS_H

#include <QObject>

class KeyboardEvents : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardEvents(QObject *parent = nullptr);

signals:

public slots:
};

#endif // KEYBOARDEVENTS_H