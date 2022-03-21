#include "net_udp.h"

Net_Udp::Net_Udp(QObject *parent)
    : QObject{parent}
{
    udpSocket = new QUdpSocket(this);
}

bool Net_Udp::bind(int port, const QHostAddress &address)
{
    return udpSocket->bind(address, port, QUdpSocket::ShareAddress);
}

bool Net_Udp::writeDatagram(const QByteArray &datagram, const QHostAddress &host, quint16 port)
{
    int ret = udpSocket->writeDatagram(datagram, host, port);
    if(ret < 0) {ret = 0; qCritical() << udpSocket->errorString();}
    return ret;
}

bool Net_Udp::broadcastDatagram(const QByteArray &datagram, quint16 port)
{
    return udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, port);
}

QByteArray Net_Udp::readDatagram(QHostAddress *host)
{
    QByteArray datagram;
    if (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        int ret = udpSocket->readDatagram(datagram.data(), datagram.size(), host);
        if(ret < 0) qCritical() << udpSocket->errorString();
    }
    return datagram;
}
