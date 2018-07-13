#ifndef KEYBOARDEVENTS_H
#define KEYBOARDEVENTS_H

#include <QObject>

class KeyboardEvents : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardEvents(QObject *parent = nullptr);
    virtual ~KeyboardEvents();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // KEYBOARDEVENTS_H
