/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ssdp_server.h"

Ssdp_Server::Ssdp_Server(QObject *parent) : QObject{parent}
{
    mPort = 16125;
    mSocket = new QUdpSocket(this);
    mAddress = QHostAddress("239.255.43.21");
    auto ok = mSocket->bind(QHostAddress::AnyIPv4, mPort, QUdpSocket::ShareAddress);
    if(ok) ok = mSocket->joinMulticastGroup(mAddress);
    if(!ok) qDebug() << "Error: SSDP Server" << mSocket->errorString();
}

bool Ssdp_Server::write(const QVariant &var)
{
    bool ret = true;
    auto rcv = mSocket->writeDatagram(var.toByteArray(), mAddress, mPort+1); mSocket->flush();
    if(rcv < 0) { ret = false; qDebug() << "Error: SSDP Server write" << mSocket->errorString();}
    return ret;
}

QStringList Ssdp_Server::respondList(const QByteArray &key)
{
    QSet<QString> set;
    QByteArray reply; QHostAddress host;
    for(int i=0; i<1000; i+=100) {
        while (mSocket->hasPendingDatagrams()) {
            reply.resize(mSocket->pendingDatagramSize());
            mSocket->readDatagram(reply.data(), reply.size(), &host);
            if(reply.contains(key)) {
                QString ip = host.toString();
                if(!set.contains(ip)) {
                    emit targetSig(ip);
                    set << ip;
                    i = 800;
                }
            }
        } /*if(set.size()) break; else */ cm::mdelay(100);
    }

    return set.values();
}

QStringList Ssdp_Server::searchTarget(const QString &room)
{
    QStringList ls;
    QString message("M-SEARCH * HTTP/1.1\r\n"        \
                    "Host:239.255.43.21:1900\r\n" \
                    "ST:urn:schemas-upnp-org:device:Pdu:%1\r\n" \
                    "Man:\"ssdp:discover\"\r\n" \
                    "MX:3\r\n" \
                    "\r\n");
    bool ret = write(message.arg(room));
    if(ret) ls = respondList(room.toLatin1());
    return ls;
}
