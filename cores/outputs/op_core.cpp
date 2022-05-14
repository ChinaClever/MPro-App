/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_core.h"

OP_Core::OP_Core(QObject *parent)
    : OP_ZRtu{parent}
{
    mThread = new CThread(this);
    QTimer::singleShot(5,this,SLOT(initFunSlot()));
}

OP_Core *OP_Core::bulid(QObject *parent)
{
    static OP_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new OP_Core(parent);
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
    mThread->init(this, SLOT(run()));
     mThread->start();
}

