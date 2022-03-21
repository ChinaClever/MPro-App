#include "ssdp_server.h"
#include <thread>

Ssdp_Server::Ssdp_Server(QObject *parent) : Ssdp_Client{parent}
{
    mPort = 16125;
    mSocket = new QUdpSocket(this);
    mAddress = QHostAddress("239.255.43.21");
    auto ok = mSocket->bind(QHostAddress::AnyIPv4, mPort, QUdpSocket::ShareAddress);
    if(ok) ok = mSocket->joinMulticastGroup(mAddress);
    if(!ok) qDebug() << "Error: SSDP Server" << mSocket->errorString();
}

Ssdp_Server *Ssdp_Server::bulid(QObject *parent)
{
    static Ssdp_Server* sington = nullptr;
    if(!sington) sington = new Ssdp_Server(parent);
    return sington;
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
            if(reply.contains(key)) set << host.toString();
        } if(set.size()) break; else cm::mdelay(100);
    }

    return set.values();
}

QStringList Ssdp_Server::searchTarget()
{
    QStringList ls;
    QByteArray message("M-SEARCH * HTTP/1.1\r\n"        \
                       "Host:239.255.43.21:1900\r\n" \
                       "ST:urn:schemas-upnp-org:device:Pdu:1\r\n" \
                       "Man:\"ssdp:discover\"\r\n" \
                       "MX:3\r\n" \
                       "\r\n");
    bool ret = write(message);
    if(ret) ls = respondList("ok");
    return ls;
}
