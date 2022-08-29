/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_netaddr.h"
#include <QNetworkInterface>

App_NetAddr::App_NetAddr(QObject *parent)
    : QObject{parent}
{
    initFun();
    setInterface();
}


App_NetAddr *App_NetAddr::bulid(QObject *parent)
{
    static App_NetAddr* sington = nullptr;
    if(sington == nullptr) {
        sington = new App_NetAddr(parent);
    }
    return sington;
}

void App_NetAddr::initFun()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    if(!strlen(net->name)) {
        sNetAddr *inet = &net->inet;
        inet->en = 1; inet->mode = 0;
        qstrcpy(inet->gw, "192.168.1.1");
        qstrcpy(inet->ip, "192.168.1.99");
        qstrcpy(inet->mask, "255.255.255.0");
        qstrcpy(net->name, "eth0");
    } net->inet.en = 1;
}

void App_NetAddr::setInterface()
{
    if(!isRun) {
        isRun = true;
        QTimer::singleShot(755,this,SLOT(updateInterface()));
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
        QTimer::singleShot(555,this,SLOT(setInterfaceSlot()));
#endif
    }
}

void App_NetAddr::setInterfaceSlot()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    if(strlen(net->name)) {
        if(net->inet.mode) {
            dhcp(net->name);
        } else {
            setIpV4();
        }
    }

    isRun = false;
}

void App_NetAddr::setIpV4()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    QString fn = net->name;
    QString ip = net->inet.ip;
    QString mask = net->inet.mask;
    QString cmd = "ifconfig %1 %2 netmask %3";
    QString str = cmd.arg(fn, ip, mask);
    system(str.toStdString().c_str());
}

void App_NetAddr::dhcp(const QString &n)
{
    QString cmd = "dhclient " + n;
    system(cmd.toStdString().c_str());
}

void App_NetAddr::updateInterface()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    QList<QNetworkInterface>list = QNetworkInterface::allInterfaces();//获取所有网络接口信息
    foreach(QNetworkInterface interface, list) {  //便利每一个接口信息
        if(interface.name() == "lo") continue;
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
                    break;
                }
            }
        }  break;
    }
}
