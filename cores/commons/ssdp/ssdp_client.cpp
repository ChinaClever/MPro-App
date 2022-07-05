/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ssdp_client.h"

Ssdp_Client::Ssdp_Client(QObject *parent)
    : QObject{parent}
{
    mPort = 16126;
    mSocket = new QUdpSocket(this);
    mAddress = QHostAddress("239.255.43.21");
    auto ok = mSocket->bind(QHostAddress::AnyIPv4, mPort, QUdpSocket::ShareAddress);
    if(ok) ok = mSocket->joinMulticastGroup(mAddress);
    if(ok) connect(mSocket,SIGNAL(readyRead()),this,SLOT(readMsgSlot()));
    else qDebug() << "Error: SSDP Client" << mSocket->errorString();
}


Ssdp_Client *Ssdp_Client::bulid(QObject *parent)
{
    static Ssdp_Client* sington = nullptr;
    if(!sington) sington = new Ssdp_Client(parent);
    return sington;
}


bool Ssdp_Client::write(const QVariant &var)
{
    bool ret = true;
    auto rcv = mSocket->writeDatagram(var.toByteArray(), mAddress, mPort-1);  mSocket->flush();
    if(rcv < 0) { ret = false; qDebug() << "Error: SSDP Client write" << mSocket->errorString();}
    return ret;
}

void Ssdp_Client::readMsgSlot()
{
    QByteArray reply;
    while (mSocket->hasPendingDatagrams()) {
        reply.resize(mSocket->pendingDatagramSize());
        mSocket->readDatagram(reply.data(), reply.size());
        recvMsg(reply);
    }
}

bool Ssdp_Client::rplySearchTarget(const QString &room, const QByteArray &array)
{
    bool ret = false;
    if(array.contains("ssdp:discover")) {
        if(array.contains("Pdu:all")) {
            ret = write("Pdu:all");
        } else if(room.size()){
            QString name = cm::masterDev()->uut.room;
            if(name == room) {
                QString str = "Pdu:"+ name;
                ret = write(str.toLatin1());
            }
        }
    }

    return ret;
}

void Ssdp_Client::recvMsg(QByteArray &array)
{
    QDataStream out(&array, QIODevice::ReadOnly); sSdpIt it;
    out >> it.version >> it.fc >> it.describe >> it.array >> it.crc;
    if(it.crc == END_CRC) {
        if(it.fc) emit recvSig(it.fc, it.describe, it.array);
        else rplySearchTarget(it.describe, it.array);
    } else qDebug() << Q_FUNC_INFO << it.fc << it.crc << array.size();
}

