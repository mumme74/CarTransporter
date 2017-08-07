#include "cannodes.h"
#include "can_protocol.h"
#include "control.h"


// -------------------------------------------------------------------------------

CanAbstractNode::CanAbstractNode(CanInterface *canInterface, QObject *parent) :
    QObject(parent),m_canIface(canInterface)
{
    connect(canInterface, SIGNAL(recievedFrames(QList<QCanBusFrame>&)),
            this, SLOT(updatedFromCan(QList<QCanBusFrame>&)));
}

CanAbstractNode::~CanAbstractNode()
{
}

QList<const CanPid *> CanAbstractNode::getAllPids() const
{
    QList<const CanPid*> list;
    foreach(CanPid *pid, m_pids) {
        list.append(pid);
    }

    return list;
}

void CanAbstractNode::setPidsValue(QString key, QString value)
{
    if (!m_pids.keys().contains(key)) {
        CanPid *pid = new CanPid(key, value, this);
        m_pids[key] = pid;
        emit newPid(pid);
    } else {
        m_pids[key]->setValue(value);
    }
}

void CanAbstractNode::setAmpPid(QString key, quint8 amp)
{
    QString value = QString::number(amp);
    value += "A";
    setPidsValue(key, value);
}

void CanAbstractNode::setVoltPid(QString key, quint16 volt)
{
    double fVolt = volt / 1000;
    QString value = QString::number(fVolt, 'g', 1);
    value += "V";
    setPidsValue(key, value);
}

void CanAbstractNode::setBoolPid(QString key, quint8 vlu)
{
    QString value = vlu != 0 ? "On" : "Off";
    setPidsValue(key, value);
}

void CanAbstractNode::setTempPid(QString key, quint8 temp)
{
    QString value = QString::number(temp);
    value += "°C";
    setPidsValue(key, value);
}



// --------------------------------------------------------------------------------

CanParkbrakeNode::CanParkbrakeNode(CanInterface *canInterface, QObject *parent) :
    CanAbstractNode(canInterface, parent)
{
}

CanParkbrakeNode::~CanParkbrakeNode()
{
}

bool CanParkbrakeNode::hasProperty(const QString &key)
{
    return m_pids.keys().contains(key);
}

void CanParkbrakeNode::updatedFromCan(QList<QCanBusFrame> &frames)
{
    if (frames.size() > 0) {
        for (const QCanBusFrame &frame : frames) {
            quint32 sid = frame.frameId();
            if ((sid & CAN_MSG_TYPE_UPDATE_MASK) != CAN_MSG_TYPE_UPDATE)
                continue;
            if ((sid & CAN_MSG_SENDER_ID_MASK) != C_parkbrakeNode)
                continue;

            QByteArray data = frame.payload();
            quint16 msgId = sid & CAN_MSG_ID_MASK;

            // LF, RF, LR, RR
            // 0   1   2   3  <- byte orders
            switch (msgId) {
            case C_parkbrakeUpdPID_1:
                // state wheels brakes | revs / sec on wheels
                // [0:7][0:7][0:7][0:7] [0:7][0:7][0:7][0:7]
                //  LF   RF   LR   RR    LF    RF  LR   RR
                setStatePid(tr("LeftFront_state"), data[0]);
                setStatePid(tr("RightFront_state"), data[1]);
                setStatePid(tr("LeftRear_state"), data[2]);
                setStatePid(tr("RightRear_state"), data[3]);

                setWheelRevs(tr("LeftFront_revs"), data[4]);
                setWheelRevs(tr("RightFront_revs"), data[5]);
                setWheelRevs(tr("LeftRear_revs"), data[6]);
                setWheelRevs(tr("RightRear_revs"), data[7]);
                break;
            case C_parkbrakeUpdPID_2:
                // amps momentary      |  amps maximum this seq.
                // [0:7][0:7][0:7][0:7] [0:7][0:7][0:7][0:7]
                //  LF   RF   LR   RR    LF   RF   LR   RR
                setAmpPid(tr("LeftFront_motor"), data[0]);
                setAmpPid(tr("RightFront_motor"), data[1]);
                setAmpPid(tr("LeftRear_motor"), data[2]);
                setAmpPid(tr("RightRear_motor"), data[3]);
                // maximum
                setAmpPid(tr("LeftFront_motor"), data[4]);
                setAmpPid(tr("RightFront_motor"), data[5]);
                setAmpPid(tr("LeftRear_motor"), data[6]);
                setAmpPid(tr("RightRear_motor"), data[7]);

                break;
            case C_parkbrakeUpdPID_3: {
                // bat volt*1000|ign volt*1000|chip temp
                //   [0:15]         [0:15]    i[0:7] -> continue on next row

                // IGN|B.LIGHTS|BTN|BTN_INV|LF_LIM|RF_LIM|LR_LIM|RR_LIM
                // [7 -   6   -  5  -  4  -   3  -   2  -  1  -   0  ]
                //  bits so the whole data is 5bytes + 8bit = 6bytes
                quint16 vlu = (data[1] << 8) | data[0];
                setVoltPid(tr("Ignition"), vlu);

                vlu = (data[1] << 8) | data[0];
                setVoltPid(tr("Battery"), vlu);

                setTempPid(tr("ChipTemp"), data[4]);

                setBoolPid(tr("Ignition"), data[5] & 0x80);
                setBoolPid(tr("Brakelights"), data[5] & 0x40);
                setBoolPid(tr("Button"), data[5] & 0x20);
                setBoolPid(tr("Button inv"), data[5] & 0x10);
                setBoolPid(tr("LF_currentLimit"), data[5] & 0x08);
                setBoolPid(tr("RF_currentLimit"), data[5] & 0x04);
                setBoolPid(tr("LR_currentLimit"), data[5] & 0x02);
                setBoolPid(tr("RR_currentLimit"), data[5] & 0x01);

             }   break;
            default:
                // unknown
                QString key = QString::number(sid, 16);
                QString vlu;
                for (quint8 v : data) {
                    vlu += QString::number(v, 16);
                }
                setPidsValue(key, vlu);
                break;
            }
        }
    }
}


void CanParkbrakeNode::setStatePid(QString key, quint8 state)
{
    QString value;
    switch (static_cast<ctrl_states>(state)) {
    case Tightened:
        value = tr("Tightened");
        break;
    case Tightening:
        value = tr("Tightening");
        break;
    case Released:
        value = tr("Released");
        break;
    case Releasing:
        value = tr("Releasing");
        break;
    case InServiceState:
        value = tr("ServiceState");
        break;
    case SetServiceState:
        value = tr("ToServiceState");
        break;
    case ErrorState:
        value = tr("ErrorState");
        break;
    }

    setPidsValue(key, value);
}

void CanParkbrakeNode::setWheelRevs(QString key, quint8 rev)
{
    QString value = QString::number(rev);
    setPidsValue(key, value);
}


