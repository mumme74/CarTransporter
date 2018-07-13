#include "caninterface.h"
#include <QCanBusDevice>
#include <QCanBus>
#include <QSettings>
#include <QtCore>

static const char can_pluginKey[] = "can_plugin";
static const char can_defaultPlugin[] = "socketcan";
static const char can_interfaceKey[] = "can_interface";
static const char can_defaultInterface[] = "can0";
static const char can_rawFilterKey[] = "can_rawFilter";
static const char can_errorFilterKey[] = "can_errorFilter";
static const char can_loopbackKey[] = "can_loopback";
static const char can_quietModeKey[] = "can_quietMode";
static const char can_bitrateKey[] = "can_bitrate";
static const char can_fdFramesKey[] = "can_fdFrames";


CanInterface::CanInterface(QObject *parent) :
    QObject(parent),
    m_canDevice(Q_NULLPTR)
{
}

CanInterface::~CanInterface()
{
    if (m_canDevice != Q_NULLPTR) {
        m_canDevice->disconnectDevice();
        m_canDevice->deleteLater();
    }
}


bool CanInterface::createInterface()
{
    if (m_canDevice != Q_NULLPTR) {
        m_canDevice->disconnectDevice();
        m_canDevice->deleteLater();
        m_canDevice = Q_NULLPTR;
        emit connectedChanged(false);
    }

    // find the can plugin
    QSettings settings;
    QString pluginName = settings.value(can_pluginKey, can_defaultPlugin).toString();
    QStringList plugins = QCanBus::instance()->plugins();
    int idx = plugins.indexOf(pluginName.toLocal8Bit());
    if (idx == -1) {
        setErrorString("Can't find the CAN plugin: " + pluginName);
        qCritical() << qPrintable(m_errorString.toLocal8Bit()) << endl;
        return false;
    }

    // try to create interface
    QString ifaceName = settings.value(can_interfaceKey, can_defaultInterface).toString();
    m_canDevice =  QCanBus::instance()->createDevice(plugins.at(idx), ifaceName.toLocal8Bit());
    if (m_canDevice == Q_NULLPTR) {
        setErrorString("Can't create device");
        qCritical() << qPrintable(m_errorString) << endl;
        return false;
    }

    // now we have a interface, connect signals
    connect(m_canDevice, &QCanBusDevice::errorOccurred,
            this, &CanInterface::receiveError);
    connect(m_canDevice, &QCanBusDevice::framesReceived,
            this, &CanInterface::checkMessages);
    connect(m_canDevice, &QCanBusDevice::framesWritten,
            this, &CanInterface::framesWritten);


    // This configuration determines the type of CAN bus frames that the current device accepts.
    // The expected value is QList<QCanBusDevice::Filter>.
    // Passing an empty list clears all previously set filters including default filters.
    if (settings.contains(can_rawFilterKey)) {
        if (m_canDevice->configurationKeys().contains(QCanBusDevice::RawFilterKey)) {
            QList<QVariant> rawfilter = settings.value(can_rawFilterKey).toList();
            m_canDevice->setConfigurationParameter(QCanBusDevice::RawFilterKey, rawfilter);
        } else {
            qDebug() << "Cant set RawFilter to interface " <<
                        qPrintable(settings.value(can_interfaceKey, can_defaultInterface).toString().toLocal8Bit()) << endl;
        }
    }

    // QCanBusDevice::ErrorFilterKey	1
    // This key defines the type of error that should be forwarded via the current connection.
    // The associated value should be of type QCanBusFrame::FrameErrors.
    if (settings.contains(can_errorFilterKey)) {
        if (m_canDevice->configurationKeys().contains(QCanBusDevice::ErrorFilterKey)) {
            QList<QVariant> errorfilter = settings.value(can_errorFilterKey).toList();
            m_canDevice->setConfigurationParameter(QCanBusDevice::ErrorFilterKey, errorfilter);
        } else {
            qDebug() << "Cant set ErrorFilter to interface " <<
                        qPrintable(settings.value(can_interfaceKey, can_defaultInterface).toString().toLocal8Bit()) << endl;
        }
    }

    // QCanBusDevice::LoopbackKey	2
    // This key defines whether the CAN bus device should operate in loopback mode.
    // The expected value for this key is bool.
    if (settings.contains(can_loopbackKey) &&
            settings.value(can_interfaceKey, can_defaultInterface).toString() != "vcan0") // we must have loopback on virtual can
    {
        if (m_canDevice->configurationKeys().contains(QCanBusDevice::LoopbackKey)) {
            m_canDevice->setConfigurationParameter(QCanBusDevice::LoopbackKey,
                                                   settings.value(can_loopbackKey).toBool());
        } else {
            qDebug() << "Cant set Loopback to interface " <<
                        qPrintable(settings.value(can_interfaceKey, can_defaultInterface).toString().toLocal8Bit()) << endl;
        }
    }


    // QCanBusDevice::ReceiveOwnKey	3
    // This key defines whether this CAN device can send messages. The expected value for this key is bool.
    if (settings.contains(can_quietModeKey)) {
        if (m_canDevice->configurationKeys().contains(QCanBusDevice::ReceiveOwnKey)) {
            // note that this configuration is inverted from our settings concept
            m_canDevice->setConfigurationParameter(QCanBusDevice::ReceiveOwnKey,
                                                   settings.value(can_quietModeKey).toBool());
        } else {
            qDebug() << "Cant set quietMode to interface " <<
                        qPrintable(settings.value(can_interfaceKey, can_defaultInterface).toString().toLocal8Bit()) << endl;
        }
    }

    // QCanBusDevice::BitRateKey	4
    // This key defines the bitrate in bits per second.
    if (settings.contains(can_bitrateKey)) {
        if ( m_canDevice->configurationKeys().contains(QCanBusDevice::BitRateKey)) {
            m_canDevice->setConfigurationParameter(QCanBusDevice::BitRateKey, settings.value(can_bitrateKey).toInt());
        } else {
            qDebug() << "Cant set Bitrate to interface " <<
                        qPrintable(settings.value(can_interfaceKey, can_defaultInterface).toString().toLocal8Bit()) << endl;
        }
    }

    // QCanBusDevice::CanFdKey	5
    // This key defines whether sending and receiving of CAN FD frames should be enabled.
    // The expected value for this key is bool.
    if (settings.contains(can_fdFramesKey)) {
        if (m_canDevice->configurationKeys().contains(QCanBusDevice::CanFdKey)) {
            m_canDevice->setConfigurationParameter(QCanBusDevice::CanFdKey, settings.value(can_fdFramesKey).toBool());
        } else {
            qDebug() << "Cant set fdFrames to interface " <<
                        qPrintable(settings.value(can_interfaceKey, can_defaultInterface).toString().toLocal8Bit()) << endl;
        }
    }

    return true;
}

bool CanInterface::connected() const
{
    if (m_canDevice == Q_NULLPTR)
        return false;

    return m_canDevice->state() == QCanBusDevice::ConnectedState;
}

bool CanInterface::setConnected(bool connect)
{
    if (connect) {
        if (m_canDevice == Q_NULLPTR) {
            if (!createInterface()) {
                emit connectedChanged(false);
                return false;
            }
        }


        // already connected
        if (m_canDevice->state() == QCanBusDevice::ConnectedState ||
            m_canDevice->state() == QCanBusDevice::ConnectingState)
        {
            return true;
        }

        // try to connect
        if (!m_canDevice->connectDevice()) {
            setErrorString( m_canDevice->errorString());
            qCritical() << qPrintable(m_errorString) << endl;

            delete m_canDevice;
            m_canDevice = Q_NULLPTR;
            emit connectedChanged(false);
            return false;
        }

        // a error might occur in configuration that isn't caught in another way
        emit connectedChanged(true);

    } else if (m_canDevice != Q_NULLPTR) {
        m_canDevice->disconnect();
        m_canDevice->deleteLater();
        m_canDevice = Q_NULLPTR;
        emit connectedChanged(false);
        clearErrorString();
    }

    return true;
}

QString CanInterface::errorString()
{
    if (m_errorString.isEmpty() && m_canDevice != Q_NULLPTR)
        setErrorString(m_canDevice->errorString());

    return m_errorString;
}

void CanInterface::clearErrorString()
{
    m_errorString.clear();
}

QVariantList CanInterface::availablePlugins() const
{
    QVariantList returnList;
    QStringList list = QCanBus::instance()->plugins();
    for (QString &plugin : list) {
        returnList.append(QVariant(plugin));
    }

    return returnList;
}

bool CanInterface::sendFrame(QCanBusFrame &frame)
{
    static bool displayErr = true;
    if (m_canDevice == Q_NULLPTR ||
            m_canDevice->state() != QCanBusDevice::ConnectedState)
    {
        // only print once
        if (displayErr) {
            setErrorString("device isn't connected");
            qWarning() << qPrintable(m_errorString) << endl;
            displayErr = false;
        }
        return false;
    }
    // only use simplest possible network in this application
    frame.setExtendedFrameFormat(false);
    frame.setFlexibleDataRateFormat(false);

    bool success = m_canDevice->writeFrame(frame);
    if (!success && displayErr) {
        setErrorString(m_canDevice->errorString());
        qWarning() << qPrintable(m_errorString) << endl;
        displayErr = false;
    }

    return success;
}

void CanInterface::receiveError(QCanBusDevice::CanBusError error)
{
    QString errName;
    switch (error) {
    case QCanBusDevice::ReadError:
        errName = "read error"; break;
    case QCanBusDevice::WriteError:
        errName = "write error"; break;
    case QCanBusDevice::ConnectionError:
        errName = "connection error"; break;
    case QCanBusDevice::ConfigurationError:
        errName = "configuration error";break;
    case QCanBusDevice::UnknownError:
        errName = "unknown error"; break;
    default: // QCanBusDevice::NoError
        return;
    }

    setErrorString(QString("%1: %2").arg(errName)
                   .arg(m_canDevice->errorString()));
    emit errorOccurred(errName, m_canDevice->errorString());
    qWarning() << qPrintable(m_errorString) << endl;
}

void CanInterface::checkMessages()
{
    if (!m_canDevice)
        return;

    QList<QCanBusFrame> frames;

    quint64 frameCount = m_canDevice->framesAvailable();
    for (quint64 i = 0; i < frameCount; ++i) {
        frames.push_back(m_canDevice->readFrame());
    }

    emit recievedFrames(frames);
}

void CanInterface::setErrorString(const QString errorString)
{
    QSettings settings;
    m_errorString = QString("%1 %2 -> %3").arg(settings.value(can_pluginKey, can_defaultPlugin).toString())
            .arg(settings.value(can_interfaceKey, can_defaultInterface).toString())
            .arg(errorString);
    emit errorStringChanged(m_errorString);
}


