/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_core.h"

Cascade_Core::Cascade_Core(QObject *parent)
    : Cascade_Slave{parent}
{
    QtConcurrent::run(this,&Cascade_Core::run);
}

Cascade_Core *Cascade_Core::bulid(QObject *parent)
{
    static Cascade_Core* sington = nullptr;
    if(sington == nullptr) sington = new Cascade_Core(parent);
    return sington;
}

void Cascade_Core::initFunSlot()
{
    if(cm::masterDev()->cfg.param.devMode < DevMode::DM_Rtu) {
        qint32 baudRate = QSerialPort::Baud57600; cm::mdelay(200);
        openSerial("/dev/ttyS1", baudRate , QSerialPort::EvenParity);
    }
}

void Cascade_Core::workFun()
{
    uchar addr = getAddress();
    cmsWrite(95); if(addr) {
        QByteArray rcv = readSerial();
        if(rcv.size() > 6) {
            rcv = qUncompress(rcv);
            QVector<c_sFrame> its = replyData(rcv);
            for(auto &it: its) workDown(it);
        } //cout << rcv.size();
    } else {
        ota_updates();
        masterReadDevs();
    }
}

void Cascade_Core::run()
{
    cm::mdelay(210);while(isRun) {
        int mode = cm::masterDev()->cfg.param.devMode;
        if(mode < DevMode::DM_Rtu) {
            if(isOpened()) workFun(); else initFunSlot();
        } else if(isOpened()) closeSerial();
        else cm::mdelay(1);
    }
}
