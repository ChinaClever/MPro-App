/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_receiver.h"
#include "sercret_core.h"

Integr_Receiver::Integr_Receiver(QObject *parent)
    : Integr_PushThread{parent}
{
    mUdp = new Net_Udp(this);
    mWs = new WS_Server(this);
    mWss = new WS_Server(this);
    mTcp = new Net_TcpServer(this); initRecvFun();
    connect(mUdp, &Net_Udp::recvSig, this, &Integr_Receiver::recvUdpSlot);
    connect(mTcp, &Net_TcpServer::recvSig, this, &Integr_Receiver::recvSlot);
    connect(mWs, &WS_Server::binaryMessageSig, this, &Integr_Receiver::recvSlot);
    connect(mWss, &WS_Server::binaryMessageSig, this, &Integr_Receiver::recvSlot);
    QtConcurrent::run(this,&Integr_Receiver::workDown);
}

Integr_Receiver::~Integr_Receiver()
{
    isRun = false;
}

void Integr_Receiver::initRecvFun()
{
    sPushCfg *cfg = &pushCfg;
    switch(cfg->recvEn) {
    case 1: mUdp->bind(cfg->recvPort); break;
    case 2: mTcp->listen(cfg->recvPort); break;
    case 3: mWs->initServer(cfg->recvPort); break;
    case 4: mWss->initSslServer(cfg->recvPort); break;
    }
}

void Integr_Receiver::closeServers()
{
    mUdp->close();
    mTcp->close();
    mWs->close();
    mWss->close();
}

void Integr_Receiver::startRecv(int en)
{
    closeServers();
    pushCfg.recvEn = en;
    initRecvFun();
}

void Integr_Receiver::setRecvPort(int port)
{
    if(pushCfg.recvPort != port) {
        pushCfg.recvPort = port;
        closeServers();
        initRecvFun();
    }
}

void Integr_Receiver::recvSlot(const QByteArray &array)
{
    Integr_JsonRecv::bulid(this)->recv(array);
}

void Integr_Receiver::recvUdpSlot(const QByteArray &array)
{
    if(Sercret_Core::cfg.type) {
        mList << array;
    } else recvSlot(array);
}

void Integr_Receiver::workDown()
{
    while(isRun) {
        if (mList.size()) {
            QByteArray array = mList.takeFirst();
            QByteArray res = Sercret_Core::bulid()->decrypt(array);
            if(res.size()) recvSlot(res);
        } else cm::mdelay(1);
    }
}
