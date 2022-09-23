/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_core.h"

sModbusSetting Mb_Core::modbusCfg;
Mb_Core::Mb_Core(QObject *parent) : QThread{parent}
{    
    mRtu = new Mb_Update(this);  mTcp = new Mb_Update(this); mCfg = &modbusCfg;
    connect(this, &Mb_Core::connectTcpSig, this, &Mb_Core::connectTcpSlot);
    connect(this, &Mb_Core::connectRtuSig, this, &Mb_Core::connectRtuSlot);
}

Mb_Core *Mb_Core::bulid(QObject *parent)
{
    static Mb_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Mb_Core(parent);
        QtConcurrent::run(sington,&Mb_Core::run);
        QTimer::singleShot(1,sington,SLOT(initFunSlot()));
    }
    return sington;
}

void Mb_Core::initFunSlot()
{
    // mCfg->enTcp = 1;
    // mCfg->port = 1502;
    emit connectTcpSig();
    emit connectRtuSig();
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


void Mb_Core::connectTcpSlot()
{
    bool ret = false;
    mTcp->disconnectModbus();
    if(mCfg->enTcp) ret =mTcp->connectTcp(mCfg->port);
    if(ret) mTcp->mbUpdates();
}

void Mb_Core::connectRtuSlot()
{     
    bool ret = false;mRtu->disconnectModbus();
    int res = cm::masterDev()->cfg.param.devMode;
    if(res == DevMode::DM_Rtu && mCfg->enRtu) {
        ret = mRtu->connectRtu(*mCfg);
    } if(ret) mRtu->mbUpdates();
}

void Mb_Core::run()
{
    cm::mdelay(20);
    bool ret = true; while (isRun) {
        ret = mRtu->isConnectedModbus();
        cm::mdelay(500); if(ret) mRtu->mbUpdates();

        ret = mTcp->isConnectedModbus();
        cm::mdelay(500); if(ret) mTcp->mbUpdates();
    }
}
