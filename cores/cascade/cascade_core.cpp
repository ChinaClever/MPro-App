/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_core.h"

Cascade_Core::Cascade_Core(QObject *parent)
    : Cascade_Slave{parent}
{
    mThread = new CThread(this);
}

Cascade_Core *Cascade_Core::bulid(QObject *parent)
{
    static Cascade_Core* sington = nullptr;
    if(sington == nullptr) {
        qint32 baudRate = QSerialPort::Baud38400;
        sington = new Cascade_Core(parent);
#if defined(Q_OS_LINUX)
        sington->openSerial("/dev/ttyUSB1", baudRate);
#else
        sington->openSerial("COM22", baudRate);
#endif
    }
    return sington;
}

void Cascade_Core::startFun()
{
    if(!isInit) isInit = mThread->init(this, SLOT(run()));
    if(!isRun) {isRun=true; mThread->start();}
}

void Cascade_Core::run()
{
    while(isRun) {
         cm::mdelay(1); cmsWriteSlot();
         uchar addr = getAddress(); if(addr) {
            QByteArray rcv = readSerial();
            if(rcv.size() > 6) {
                QVector<c_sFrame> its = replyData(rcv);
                for(auto &it: its) workDown(it);
            }
        } else {
             ota_updates();
             masterReadDevs();
         }
    }
}
