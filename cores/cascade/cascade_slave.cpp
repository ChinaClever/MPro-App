#include "cascade_slave.h"

Cascade_Slave::Cascade_Slave(QObject *parent) : Cascade_Fill{parent}
{
    mAddr = 0;
    mThread = new CThread(this);
    mThread->init(this, SLOT(run()));
}

Cascade_Slave *Cascade_Slave::bulid(QObject *parent)
{
    static Cascade_Slave* sington = nullptr;
    if(sington == nullptr) {
        sington = new Cascade_Slave(parent);
    }
    return sington;
}

void Cascade_Slave::setAddress(int addr)
{
    mAddr=addr;
    mThread->start();
    // mThread->stop();
}

bool Cascade_Slave::replyDevData(uchar fc)
{
    QByteArray array = toDataStream();
    return writeData(fc, 0, array);
}


bool Cascade_Slave::replyAlarm(QByteArray &rcv)
{
    sSetAlarmUnit unit = cm::toStruct<sSetAlarmUnit>(rcv);
    unit.index.addr = 0;
    return setAlarm(unit);
}


bool Cascade_Slave::replyRelayCtrl(QByteArray &rcv)
{
    return outputRelayCtrl(0, rcv[0], rcv[1]);
}

bool Cascade_Slave::replyRelaySet(QByteArray &rcv)
{
    return outputRelaySet(0, rcv[0], rcv[1], rcv[2]);
}

bool Cascade_Slave::workDown(QByteArray &rcv)
{
    bool ret = false;
    uchar fc, addr; ushort size; QByteArray array;
    QDataStream out(&rcv, QIODevice::ReadOnly);
    out >> fc >> addr >> size; if(size > 6) out >> array;
    if((addr == mAddr) || (addr == fc_mask)) {
        switch (fc) {
        case fc_readDev: ret = replyDevData(fc); break;
        case fc_writeAlarm: ret = replyAlarm(array); break;
        case fc_relaySet: ret = replyRelaySet(array); break;
        case fc_relayCtrl: ret = replyRelayCtrl(array); break;

        case fc_otaStart: ret = otaReplyStart(array); break;
        case fc_otaEnd: ret = otaReplyFinish(array); break;
        case fc_otaPack: ret = otaReplyPacket(array); break;
        }
    }

    return ret;
}


void Cascade_Slave::run()
{
    if(mAddr) {
        QByteArray rcv = readSerial();
        if((rcv.size()>4) && crcCheck(rcv)) {
            workDown(rcv);
        } else fillData(mAddr);
    } else {ota_updates();masterReadDevs();}
    mThread->msleep(1);
}
