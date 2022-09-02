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
        //QTimer::singleShot(1,sington,SLOT(initThreadSlot()));
    }
    return sington;
}

void Cascade_Core::startFun()
{    
    if(!isRun) {
        //QTimer::singleShot(1,this,SLOT(initFunSlot())); cm::mdelay(2);
        mThread->init(this, SLOT(run()));
        isRun = true; mThread->start();
    }
}

void Cascade_Core::initFunSlot()
{
    if(cm::masterDev()->cfg.param.devMode < DevMode::DM_Rtu) {
        qint32 baudRate = QSerialPort::Baud57600;
        openSerial("/dev/ttyUSB0", baudRate, QSerialPort::EvenParity);
    }
}

void Cascade_Core::stopFun()
{
    isRun = false;
    closeSerial();
}

void Cascade_Core::workFun()
{
    uchar addr = getAddress();
    cmsWriteSlot(); if(addr) {
        QByteArray rcv = readSerial();
        if(rcv.size() > 6) {
            rcv = qUncompress(rcv);
            QVector<c_sFrame> its = replyData(rcv);
            for(auto &it: its) workDown(it);
        }
    } else {
        ota_updates();
        masterReadDevs();
    }
}

void Cascade_Core::run()
{
    while(isRun) {
        int mode = cm::masterDev()->cfg.param.devMode;
        if(mode && (mode < DevMode::DM_Rtu)) {
            if(isOpened()) workFun(); else QTimer::singleShot(1,this,SLOT(initFunSlot()));
        } else if(isOpened()) closeSerial();
        else cm::mdelay(1);
    }
}
