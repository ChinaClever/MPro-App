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
    if(cm::masterDev()->cfg.param.devMode) {
        qint32 baudRate = QSerialPort::Baud57600; cm::mdelay(200);
        openSerial("/dev/ttyS1", baudRate , QSerialPort::EvenParity);
    }
}

void Cascade_Core::cascadeSlaveLog(int addr, bool recv)
{
    static int  cnt = 0;
    if((1 == cnt++) && recv) {
        sEventItem it; it.addr = addr;
        if(cm::cn()) {
            it.event_type = tr("级联通讯");
            it.event_content = tr("副机 %1 收到异常通讯数据").arg(addr);
        } else {
            it.event_type = "Cascading communication";
            it.event_content = tr("Abnormal communication data of auxiliary machine %1").arg(addr);
        } Log_Core::bulid()->append(it);
    } else if(!recv) cnt = 0;
    if(recv) cout;
}

void Cascade_Core::workFun()
{
    uchar addr = getAddress();
    cmsWrite(195); if(addr) {
        QByteArray rcv = readSerial(); if(rcv.size() > 6) {
            rcv = qUncompress(rcv); if(rcv.size()) {
                QVector<c_sFrame> its = replyData(rcv);
                for(auto &it: its) workDown(it);
            } cascadeSlaveLog(addr, rcv.isEmpty());
        }
    } else {
        ota_updates();
        masterReadDevs();
    }
}

void Cascade_Core::run()
{
    cm::mdelay(210);while(isRun) {
        if(cm::masterDev()->cfg.param.devMode) {
            if(isOpened()) workFun(); else initFunSlot();
        } else if(isOpened()) closeSerial();
        else cm::mdelay(1);
    }
}
