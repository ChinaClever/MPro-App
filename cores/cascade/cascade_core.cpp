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
        sington = new Cascade_Core(parent);
        QTimer::singleShot(1,sington,SLOT(initThreadSlot()));
    }
    return sington;
}

void Cascade_Core::startFun()
{    
    if(!isRun) {
        QTimer::singleShot(1,this,SLOT(initFunSlot()));
        cm::mdelay(2); isRun = true; mThread->start();
    }
}

void Cascade_Core::initFunSlot()
{
    qint32 baudRate = QSerialPort::Baud57600;
    openSerial("/dev/ttyUSB0", baudRate, QSerialPort::EvenParity);
}

void Cascade_Core::stopFun()
{
    isRun = false;
    closeSerial();
}

void Cascade_Core::run()
{
    while(isRun) {
        cm::mdelay(1000); cmsWriteSlot();
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
