#include "keyboardevents.h"
#include <QKeyEvent>
#include <QCoreApplication>

KeyboardEvents::KeyboardEvents(QObject *parent) :
    QObject(parent)
{
}

KeyboardEvents::~KeyboardEvents()
{
}

bool KeyboardEvents::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
            // exit application if Ctrl+C
          QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
          if ((keyEvent->modifiers() & Qt::ControlModifier) &&
               keyEvent->key() == Qt::Key_C)
          {
              QCoreApplication::quit();
          }
          return true;
      }
      // standard event processing
      return QObject::eventFilter(obj, event);
}
