#include "cansuspensionnode.h"
#include "can_protocol.h"
#include "suspension_config.h"
#include <QDebug>


CanSuspensionNode::CanSuspensionNode(CanInterface *canInterface, QObject *parent) :
    CanAbstractNode(C_suspensionNode, canInterface, parent)
{
    // init some known PIDS
    QCanBusFrame pid1;
    pid1.setExtendedFrameFormat(false);
    pid1.setFrameId(CAN_MSG_TYPE_UPDATE | C_suspensionUpdPID_1 | C_suspensionNode);
    pid1.setPayload(QByteArray("\0\0\0\0\0\0\0\0", 8));

    QCanBusFrame pid2;
    pid2.setExtendedFrameFormat(false);
    pid2.setFrameId(CAN_MSG_TYPE_UPDATE | C_suspensionUpdPID_2 | C_suspensionNode);
    pid2.setPayload(QByteArray("\0\0\0\0\0\0\0\0", 8));

    QCanBusFrame pid3;
    pid3.setExtendedFrameFormat(false);
    pid3.setFrameId(CAN_MSG_TYPE_UPDATE | C_suspensionUpdPID_3 | C_suspensionNode);
    pid3.setPayload(QByteArray("\0\0\0\0\0\0\0\0", 8));

    QCanBusFrame pid4;
    pid4.setExtendedFrameFormat(false);
    pid4.setFrameId(CAN_MSG_TYPE_UPDATE | C_suspensionUpdPID_4 | C_suspensionNode);
    pid4.setPayload(QByteArray("\0\0\0\0\0\0", 6));

    QList<QCanBusFrame> frames;
    frames << pid1 << pid2 << pid3 << pid4;

    qmlRegisterUncreatableType<CanSuspensionNode>("mummesoft", 0, 1, "suspensionNode", QStringLiteral("Cant create CanSuspensionNode from QML"));

}

CanSuspensionNode::~CanSuspensionNode()
{
}


bool CanSuspensionNode::fetchDtc(int storedIdx, QJSValue jsCallback)
{
    return CanAbstractNode::fetchDtc(storedIdx, jsCallback, C_suspensionDiagGetDTC);
}

void CanSuspensionNode::fetchAllDtcs()
{
    CanAbstractNode::fetchAllDtcs(C_suspensionDiagDTCLength);
}

void CanSuspensionNode::clearAllDtcs()
{
    CanAbstractNode::clearAllDtcs(C_suspensionDiagClearDTC);
}

bool CanSuspensionNode::activateOutput(int wheel, bool tighten) const
{
    return false;
}

bool CanSuspensionNode::fetchSetting(quint8 idx, QJSValue jsCallback)
{
    if (idx >= Configs::ConfigEnd)
        return false;

    return CanAbstractNode::fetchSetting(idx, jsCallback, C_suspensionCmdGetConfig);
}

bool CanSuspensionNode::setSettingUint16(quint8 idx, quint16 vlu, QJSValue jsCallback)
{
    if (idx >= Configs::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingU16(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

bool CanSuspensionNode::setSettingUint32(quint8 idx, quint32 vlu, QJSValue jsCallback)
{
    if (idx >= Configs::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingU32(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

bool CanSuspensionNode::setSettingFloat(quint8 idx, float vlu, QJSValue jsCallback)
{
    if (idx >= Configs::ConfigEnd)
        return false;

    return CanAbstractNode::setSettingF(idx, vlu, jsCallback, C_suspensionCmdSetConfig);
}

void CanSuspensionNode::updateCanFrame(const QCanBusFrame &frame)
{

}

void CanSuspensionNode::commandCanFrame(const QCanBusFrame &frame)
{
    // when parkbrakenode sends a command frame
    QByteArray payload = frame.payload();
    quint32 sid = frame.frameId();
    switch (sid & CAN_MSG_ID_MASK) {
    case C_suspensionCmdGetConfig: {
        quint8 idx = payload[0];
        if (frame.payload().size() == 3) {
            quint16 vlu = (payload[2] << 8) | payload[1];
            settingsFetchArrival(idx, vlu);
        } else if (payload.size() == 5) {
            // FIXME need a better algorithm here, 32bit uint also has 4 bytes
            quint32 u_vlu = (payload[4] << 24) | (payload[3] << 16) | (payload[2] << 8) | (payload[1]);
            float vlu = (float)u_vlu;
            settingsFetchArrivalFloat(idx, vlu);
        }
    } break;
    case C_suspensionCmdSetConfig: {
        quint8 idx = payload[0];
        quint8 res = payload[1];
        settingsSetArrival(idx, res);
    } break;
    default:
        break;
    }
}

void CanSuspensionNode::exceptionCanFrame(const QCanBusFrame &frame)
{

}

void CanSuspensionNode::diagCanFrame(const QCanBusFrame &frame)
{
    // when parkbrakeNode sends a diagnose frame
    QByteArray payload = frame.payload();
    quint32 sid = frame.frameId();
    switch (sid & CAN_MSG_ID_MASK) {
    case C_suspensionDiagDTCLength:
        // response
        //  [0:7] the length(number) of stored dtc's
        if (payload.size()) {
            m_dtcCount = payload[0];
            emit dtcCountChanged(m_dtcCount);

            if (m_dtcs.size() != m_dtcCount) {
                // refetch automatically
                m_dtcs.clear();
                // fetch all others
                for (quint8 i = 0; i < m_dtcCount; ++i) {
                    QByteArray pl(i, 1);
                    QCanBusFrame f(CAN_MSG_TYPE_DIAG | C_suspensionDiagGetDTC | C_displayNode, pl);
                    f.setExtendedFrameFormat(false);
                    m_canIface->sendFrame(f);
                }
            }
        }
        break;

    case C_suspensionDiagClearDTC:
        // response  [0:7] number erased
        if (payload.size()) {
            if (m_dtcCount != payload[0]) {
                qDebug() << "dtcCount mismatch on erase, stored:"
                         << m_dtcCount << " cleared:" << payload[0] << endl;
            }
            m_dtcCount = 0;
            m_dtcs.clear();
            bool cleared = (quint8)(payload[0]) != 0;
            if (!cleared)
                cleared = m_dtcCount == 0; // when clearing a empty dtcs list
            emit dtcsCleared(cleared);
            emit dtcCountChanged(m_dtcCount);

        }

        break;

    case C_suspensionDiagGetDTC: {
        // response:
        //  [0:7]        [0:15]    [0:6] [:7]
        // stored nr     dtc code  occurrences & pending mask
        int storedNr = payload[0];
        quint16 code = (payload[2] << 8) | payload[1];
        quint8 occurences = payload[3];
        if (m_dtcs.contains(storedNr)) {
            CanDtc *dtc = m_dtcs[storedNr];
            dtc->setOccurences(occurences & 0x7F);
            dtc->setPending(occurences & 0x80); // bit7 is pending marker
        } else {
            CanDtc *dtc = new CanDtc(this, storedNr, code, occurences & 0x7F, 0);
            m_dtcs.insert(storedNr, dtc);
        }
        dtcOnArrival(storedNr);
        emit dtcArrived(storedNr);

    }   break;

    case C_suspensionDiagActuatorTest:
        // not sure if we should do anything here
        break;
    }
}
