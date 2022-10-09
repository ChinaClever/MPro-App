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
    if(sington == nullptr) sington = new OP_Core(parent);
    return sington;
}

void OP_Core::initFunSlot()
{
    qint32 baudRate = QSerialPort::Baud19200;
    openSerial("/dev/ttyS1", baudRate);
}

void OP_Core::startFun()
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    QTimer::singleShot(1,this,SLOT(initFunSlot()));
    mThread->init(this, SLOT(run()));
    cm::mdelay(3); mThread->start();
#endif
}

