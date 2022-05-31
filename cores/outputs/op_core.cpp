/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_core.h"

OP_Core::OP_Core(QObject *parent)
    : OP_ZRtu{parent}
{
    mThread = new CThread(parent);
}

OP_Core *OP_Core::bulid(QObject *parent)
{
    static OP_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new OP_Core(parent);
        qint32 baudRate = QSerialPort::Baud19200;
        sington->openSerial("/dev/ttyUSB0", baudRate);
    }
    return sington;
}

void OP_Core::initFunSlot()
{
    qint32 baudRate = QSerialPort::Baud19200;
    openSerial("/dev/ttyUSB0", baudRate);
}

void OP_Core::startFun()
{
    QTimer::singleShot(1,this,SLOT(initFunSlot()));
    mThread->init(this, SLOT(run()));
    cm::mdelay(5); mThread->start();
}

