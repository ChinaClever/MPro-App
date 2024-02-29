/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_core.h"

sModbusSetting Mb_Core::modbusCfg;
Mb_Core::Mb_Core(QObject *parent) : QThread{parent}
{
    mRtu = new Mb_Update(this);  mTcp = new Mb_Update(this);
    mMpduRtu = new Mpdurtu_Update(this);
    mMpduTcp = new Mpdurtu_Update(this);
    mCfg = &modbusCfg;
    connect(this, &Mb_Core::connectTcpSig, this, &Mb_Core::connectTcpSlot);
    connect(this, &Mb_Core::connectRtuSig, this, &Mb_Core::connectRtuSlot);
    mTimer = new QTimer(this); mTimer->start(2500);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(run()));
    QTimer::singleShot(152, this, SLOT(initFunSlot()));
}

Mb_Core *Mb_Core::bulid(QObject *parent)
{
    static Mb_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Mb_Core(parent);
    }
    return sington;
}

void Mb_Core::initFunSlot()
{
    if( cm::masterDev()->cfg.param.oldProtocol == 0 ){
        emit connectTcpSig();
        emit connectRtuSig();
    }else{
        emit connectTcpSig();
        emit connectRtuSig();
    }
}

void Mb_Core::setAddress(int addr)
{
    mCfg->addrRtu = addr;
    if( cm::masterDev()->cfg.param.oldProtocol == 0) {
        mRtu->setAddress(addr);
    }else{
        mMpduRtu->setAddress(addr);
    }
}

void Mb_Core::setTcpAddress(int addr)
{
    mCfg->addrTcp = addr;
    if( cm::masterDev()->cfg.param.oldProtocol == 0) {
        mTcp->setAddress(addr);
    }else{
        mMpduTcp->setAddress(addr);
    }
}

void Mb_Core::setPort(int port)
{
    if( cm::masterDev()->cfg.param.oldProtocol == 0) {
        bool ret = mTcp->isConnectedModbus(); mCfg->port = port;
        if(ret) mTcp->setModbus(QModbusDevice::NetworkPortParameter, port);
    }else{
        bool ret = mMpduTcp->isConnectedModbus(); mCfg->port = port;
        if(ret) mMpduTcp->setModbus(QModbusDevice::NetworkPortParameter, port);
    }
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
    } if(ret){
        if( cm::masterDev()->cfg.param.oldProtocol == 0) {
            mRtu->setModbus(parameter, value);
        }else{
            mMpduRtu->setModbus(parameter, value);
        }
    }
}

void Mb_Core::connectTcpSlot()
{
    bool ret = false;
    if( cm::masterDev()->cfg.param.oldProtocol == 0) {
        mTcp->disconnectModbus();
        if(mCfg->enTcp) ret =mTcp->connectTcp(mCfg->addrTcp, mCfg->port);
        if(ret) {
            int cnt = cm::masterDev()->cfg.nums.slaveNum;
            for(int i=0; i<cnt; ++i) mTcp->mbUpdates(i);
        }
    }else{
        mMpduTcp->disconnectModbus();
        if(mCfg->enTcp) ret =mMpduTcp->connectTcp(mCfg->addrTcp, mCfg->port);
        if(ret) {
            int cnt = 1;
            for(int i=0; i<cnt; ++i) mMpduTcp->mbUpdates(i);
        }
    }
}

void Mb_Core::connectRtuSlot()
{     
    bool ret = false;
    if( cm::masterDev()->cfg.param.oldProtocol == 0) mRtu->disconnectModbus();
    else mMpduRtu->disconnectModbus();
    int res = cm::masterDev()->cfg.param.devMode;
    if(res == EDevMode::DM_Rtu && mCfg->enRtu) {
        if( cm::masterDev()->cfg.param.oldProtocol == 0) {
            ret = mRtu->connectRtu(*mCfg);
            if(ret)  {
                mRtu->mbUpdates(0);
            }else cout << mCfg->addrRtu << ret;
        }else{
            ret = mMpduRtu->connectRtu(*mCfg);
            if(ret)  {
                mMpduRtu->mbUpdates(0);
            }else cout << mCfg->addrRtu << ret;
        }

    }
}

void Mb_Core::run()
{
    static uint cnt = 0;
    bool ret = true;
    if( cm::masterDev()->cfg.param.oldProtocol == 0) {
        if(cnt++ %2) {
            ret = mRtu->isConnectedModbus();
            if(ret) mRtu->mbUpdates(0);
        } else {
            if(mTcp->isConnectedModbus()) {
                int cnt = cm::masterDev()->cfg.nums.slaveNum;
                for(int i=0; i<=cnt; ++i) mTcp->mbUpdates(i);
            }
        }
    }else{
        if(cnt++ %2) {
            ret = mMpduRtu->isConnectedModbus();
            if(ret) mMpduRtu->mbUpdates(0);
        } else {
            if(mMpduTcp->isConnectedModbus()) {
                int cnt = 0;
                for(int i=0; i<=cnt; ++i) mMpduTcp->mbUpdates(i);
            }
        }
    }
}

