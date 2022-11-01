/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "otassdp.h"

OtaSsdp::OtaSsdp(QObject *parent)
    : Ssdp_Obj{parent}
{

}


bool OtaSsdp::checkInput(const sSdpIt &it)
{
    bool ret = true;
    if(it.room.size()) {
        QString res = cm::masterDev()->cfg.uut.room;
        if(res != it.room) ret = false;
    }

    if(it.ip.size()) {
        QString res = cm::dataPacket()->net.inet.ip;
        if(res != it.ip) ret = false;
    }

    return ret;
}

bool OtaSsdp::rplySearchTarget(const sSdpIt &it)
{
    bool ret = checkInput(it);
    if(ret && it.data.size() > 100) {
        sSdpIt item; item.fc = 0;
        inet_updateInterface();
        item.ip = cm::dataPacket()->net.inet.ip;
        item.room = cm::masterDev()->cfg.uut.room;
        item.data = QByteArray::number(cm::masterDev()->cfg.nums.slaveNum);
        ret = udpSend(item);
    }
    return ret;
}


void OtaSsdp::inet_updateInterface()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    QList<QNetworkInterface>list = QNetworkInterface::allInterfaces();//获取所有网络接口信息
    foreach(QNetworkInterface interface, list) {  //便利每一个接口信息
#if (QT_VERSION < QT_VERSION_CHECK(5,13,0))
        if(interface.name() != "eth0") continue;
#else
        if(interface.name() !="ens33") continue;
#endif
        qstrcpy(net->name, interface.name().toLatin1().constData());//设备名称
        qstrcpy(net->mac, interface.hardwareAddress().toLatin1().constData());//获取并输出mac地址
        QList<QNetworkAddressEntry>entryList=interface.addressEntries();//获取ip地址和子网掩码和广播地址
        foreach(QNetworkAddressEntry entry, entryList) {//便利ip条目列表
            QHostAddress hostIp = entry.ip();
            if(hostIp != QHostAddress(QHostAddress::LocalHost)) {
                switch (hostIp.protocol()) {
                case QAbstractSocket::IPv4Protocol:
                    net->inet.prefixLen = entry.prefixLength();//获取子网掩码
                    qstrcpy(net->inet.ip, hostIp.toString().toLatin1().constData()); //获取ip
                    qstrcpy(net->inet.mask, entry.netmask().toString().toLatin1().constData()); //获取子网掩码
                    break;

                case QAbstractSocket::IPv6Protocol:
                    qstrcpy(net->inet6.ip, hostIp.toString().toLatin1().constData()); //获取ip
                    qstrcpy(net->inet6.mask, entry.netmask().toString().toLatin1().constData()); //获取子网掩码
                    net->inet6.prefixLen = entry.prefixLength();//获取子网掩码
                    break;

                default:
                    cout << net->name << net->mac << hostIp.toString() << entry.netmask().toString();
                    break;
                }
            }
        }
    }
}
