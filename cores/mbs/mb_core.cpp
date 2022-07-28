/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_core.h"

sModbusSetting Mb_Core::modbusCfg;
Mb_Core::Mb_Core(QObject *parent)
{
    mRtu = new Mb_Update(parent);
    mTcp = new Mb_Update(parent);
    mCfg = &modbusCfg;
}

Mb_Core *Mb_Core::bulid(QObject *parent)
{
    static Mb_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Mb_Core(parent);
        //sington->connectTcp(1); /////====
    }
    return sington;
}

void Mb_Core::initFun()
{
    connectTcp(mCfg->enTcp);
    connectRtu(mCfg->enRtu);
}

void Mb_Core::setAddress(int addr)
{
    mCfg->addr = addr;
    mRtu->setAddress(addr);
}

void Mb_Core::setPort(int port)
{
    bool ret = mTcp->isConnectedModbus(); mCfg->port = port;
    if(ret) mTcp->setModbus(QModbusDevice::NetworkPortParameter, port);
}

void Mb_Core::setRtu(int parameter, const QVariant &value)
{
    bool ret = true;
    switch (parameter) {
    case QModbusDevice::SerialPortNameParameter: mCfg->portName = value.toString(); break;
    case QModbusDevice::SerialBaudRateParameter: mCfg->baud = value.toInt(); break;
    case QModbusDevice::SerialParityParameter: mCfg->parity = value.toInt(); break;
    case QModbusDevice::SerialDataBitsParameter: mCfg->dataBits = value.toInt(); break;
    case QModbusDevice::SerialStopBitsParameter: mCfg->stopBits = value.toInt(); break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    } if(ret) mRtu->setModbus(parameter, value);
}

bool Mb_Core::connectModbus(Mb_Update *mb, bool en, int rt)
{
    bool ret = true; if(en) {
        //ret = mb->isConnectedModbus();
        if(rt) ret = mb->connectTcp(mCfg->port);
        else ret = mRtu->connectRtu(*mCfg);
        if(ret) mb->mbUpdates();
    } else {
        ret = mb->isConnectedModbus();
        if(ret) mb->disconnectModbus();
    }

    return ret;
}

bool Mb_Core::connectTcp(int en)
{
    mCfg->enTcp = en;
    return connectModbus(mTcp, en, 1);
}

bool Mb_Core::connectRtu(int en)
{
    mCfg->enRtu = en;
    return connectModbus(mRtu, en, 0);
}

void Mb_Core::run()
{
    initFun();
    bool ret = true;
    while (isRun) {
        ret = mRtu->isConnectedModbus();
        cm::mdelay(500); if(ret) mRtu->mbUpdates();

        ret = mTcp->isConnectedModbus();
        cm::mdelay(500); if(ret) mTcp->mbUpdates();
    }
}
