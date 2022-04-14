#include "cascade_slave.h"

Cascade_Slave::Cascade_Slave(QObject *parent) : Cascade_Fill{parent}
{
    setAddress(1);
    mThread = new CThread(this);
}


Cascade_Slave::~Cascade_Slave()
{
    isRun = false;
    mThread->stop();
}

Cascade_Slave *Cascade_Slave::bulid(QObject *parent)
{
    static Cascade_Slave* sington = nullptr;
    if(sington == nullptr) {
        qint32 baudRate = QSerialPort::Baud38400;
        sington = new Cascade_Slave(parent);
#if defined(Q_OS_LINUX)
        sington->openSerial("/dev/ttyUSB1", baudRate);
#else
        sington->openSerial("COM22", baudRate);
#endif
        sington->start();
    }
    return sington;
}


void Cascade_Slave::start()
{
    mThread->init(this, SLOT(run()));
    isRun = true; mThread->start();
}

bool Cascade_Slave::replyDevData(uchar fc)
{
    QByteArray array = toDataStream();
    return writeData(fc, 0, array);
}


bool Cascade_Slave::replyAlarm(QByteArray &rcv)
{
    sSetAlarmUnit unit = cm::toStruct<sSetAlarmUnit>(rcv);
    unit.index.addr = 0; Set_Core *set = Set_Core::bulid();
    return set->setAlarm(unit);
}


bool Cascade_Slave::replyRelayCtrl(QByteArray &rcv)
{
    Set_Core *set = Set_Core::bulid();
    return set->outputRelayCtrl(0, rcv[0], rcv[1]);
}

bool Cascade_Slave::replyRelaySet(QByteArray &rcv)
{
    Set_Core *set = Set_Core::bulid();
    return set->outputRelaySet(0, rcv[0], rcv[1], rcv[2]);
}

bool Cascade_Slave::workDown(c_sFrame &it)
{
    bool ret = false;
    if((it.dstAddr == getAddress()) || (it.dstAddr == fc_mask)) {
        switch (it.fc) {
        case fc_readDev: ret = replyDevData(it.fc); break;
        case fc_writeAlarm: ret = replyAlarm(it.data); break;
        case fc_delaySet: ret = replyRelaySet(it.data); break;
        case fc_relayCtrl: ret = replyRelayCtrl(it.data); break;

        case fc_otaStart: ret = otaReplyStart(it.data); break;
        case fc_otaEnd: ret = otaReplyFinish(it.data); break;
        case fc_otaPack: ret = otaReplyPacket(it.data); break;
        }
    }

    return ret;
}


void Cascade_Slave::run()
{
    while(isRun) {        
         mThread->msleep(1); cmsWriteSlot();
         uchar addr = getAddress(); if(addr) {
            QByteArray rcv = readSerial();
            if(rcv.size() > 6) {
                QVector<c_sFrame> its = replyData(rcv);
                for(auto &it: its) workDown(it);
                if(its.size()) fillData(addr);
            }
        } else {
             ota_updates();
             masterReadDevs();
         }
    }
}
