/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_receiver.h"

Integr_Receiver::Integr_Receiver(QObject *parent)
    : QObject{parent}
{
    mUdp = new Net_Udp(this);
    mWs = new WS_Server(this);
    mTcp = new Net_TcpServer(this);
    connect(mUdp, &Net_Udp::recvSig, this, &Integr_Receiver::recvSlot);
    connect(mTcp, &Net_TcpServer::recvSig, this, &Integr_Receiver::recvSlot);
    connect(mWs, &WS_Server::binaryMessageSig, this, &Integr_Receiver::recvSlot);
}


void Integr_Receiver::recvSlot(const QByteArray &array)
{
    Integr_JsonRecv::bulid(this)->recv(array);
}
