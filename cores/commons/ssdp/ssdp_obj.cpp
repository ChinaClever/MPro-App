/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */

#include "ssdp_obj.h"

Ssdp_Obj::Ssdp_Obj(QObject *parent) : QObject(parent)
{
    mPort = 9126;
    mUdp = new Net_Udp(this);
    mSocket = new QUdpSocket(this);
    qRegisterMetaType<sSdpIt>("sSdpIt");
    mAddress = QHostAddress("239.255.43.21");
}

Ssdp_Obj::~Ssdp_Obj()
{
    ssdpClose();
}

void Ssdp_Obj::ssdpClose()
{
    if(isOpen) mSocket->close();
    isOpen = false;
}

bool Ssdp_Obj::ssdpBind(bool en)
{
    if(isOpen) return isOpen; //else cout << "ssdp bind port";
    system("route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0"); cm::mdelay(1);
    auto ok = mSocket->bind(QHostAddress::AnyIPv4, mPort, QUdpSocket::ShareAddress);
    if(ok) ok = mSocket->joinMulticastGroup(mAddress);
    else cout << "ssdp bind port error" << mSocket->errorString();
    if(ok) connect(mSocket,SIGNAL(readyRead()),this,SLOT(readMsgSlot()));
    else {mSocket->close(); cout << "ssdp joinMulticastGroup error" << mSocket->errorString();}
    isOpen = ok; if(!ok) QTimer::singleShot(1567,this,SLOT(reSsdpBindSlot()));
    qDebug() << "ssdp bind port" << ok << en << mPort;
    return ok;
}

bool Ssdp_Obj::udpBind()
{
    bool ret = mUdp->bind(mPort+1);
    if(ret) connect(mUdp,&Net_Udp::recvSig,this,&Ssdp_Obj::recvUdpSlot);
    return ret;
}

void Ssdp_Obj::readMsgSlot()
{
    QByteArray reply;
    while (mSocket->hasPendingDatagrams()) {
        reply.resize(mSocket->pendingDatagramSize());
        mSocket->readDatagram(reply.data(), reply.size(), &mHost);
        recvMsg(reply); //qDebug() << mHost.toString();
    }
}

QByteArray Ssdp_Obj::toArray(const sSdpIt &it)
{
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << it.version << it.fc <<  it.room << it.ip << it.target << it.data << END_CRC;
    return qCompress(array);
}

bool Ssdp_Obj::toItem(const QByteArray &array, sSdpIt &it)
{
    QByteArray data = qUncompress(array); //qDebug() << data;
    QDataStream out(&data, QIODevice::ReadOnly); bool ret = false;
    out >> it.version >> it.fc >> it.room >> it.ip >> it.target >> it.data >> it.crc;
    if(it.crc == END_CRC) ret = true;
    return ret;
}

bool Ssdp_Obj::ssdpSend(const sSdpIt &it)
{
    bool ret = true; QByteArray array = toArray(it);
    auto rcv = mSocket->writeDatagram(array, mAddress, mPort);  mSocket->flush();
    if(rcv < 0) { ret = false; cout << "Error: SSDP write" << mSocket->errorString();}
    return ret;
}

bool Ssdp_Obj::udpSend(const sSdpIt &it)
{
    QByteArray array = toArray(it);
    return mUdp->writeDatagram(array, mHost, mPort+1);
}


void Ssdp_Obj::recvMsg(const QByteArray &array)
{
    sSdpIt it; if(toItem(array, it)) {
        if(it.fc) emit recvSig(it);
        else rplySearchTarget(it);
    } else cout << it.fc << it.crc << array.size();
}
