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
}

OP_Core *OP_Core::bulid(QObject *parent)
{
    static OP_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new OP_Core(parent);
    }
    return sington;
}

void OP_Core::startFun()
{
    mThread->init(this, SLOT(run()));
    qint32 baudRate = QSerialPort::Baud19200;
    bool ret = openSerial("/dev/ttyUSB0", baudRate);
    if(ret) mThread->start();
}

