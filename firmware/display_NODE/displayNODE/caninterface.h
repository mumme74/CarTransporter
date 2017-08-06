#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include <QObject>
#include <QVariantList>
#include <QCanBusDevice>

class CanInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
public:
    explicit CanInterface(QObject *parent = 0);
    ~CanInterface();

    bool connected() const;
    bool setConnected(bool connect);

    QString errorString();
    Q_INVOKABLE void clearErrorString();
    Q_INVOKABLE QVariantList availablePlugins() const;

    bool sendFrame(const QCanBusFrame &frame);

signals:
    void connectedChanged(bool connected);
    void errorStringChanged(QString errorString);
    void recievedFrames(QList<QCanBusFrame> &frames);
    void framesWritten(qint64 count);

public slots:

private slots:
    void receiveError(QCanBusDevice::CanBusError error);
    void checkMessages();


private:
    void setErrorString(const QString errorString);
    bool createInterface();
    QCanBusDevice *m_canDevice;
    QString m_errorString;
};

#endif // CANINTERFACE_H
