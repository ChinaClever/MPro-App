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
    QTimer::singleShot(544,this,SLOT(inet_initFunSlot()));
}

void App_NetAddr::inet_initFunSlot()
{
    QString fn = Cfg_Obj::pathOfCfg("inet.ini");
    mInetCfg = new Cfg_Obj(fn, this);

    sNetInterface *net = &(cm::dataPacket()->net);
    inet_readCfg(net->inet, "IPV4"); net->inet.en = 1;
    inet_readCfg(net->inet6, "IPV6"); qstrcpy(net->name, "eth0");

    if(!strlen(net->inet.ip)) {
        sNetAddr *inet = &net->inet;
        inet->en = 1; inet->mode = 0;
        qstrcpy(inet->gw, "192.168.1.1");
        qstrcpy(inet->ip, "192.168.1.99");
        qstrcpy(inet->mask, "255.255.255.0");
    } inet_setInterface();
}

void App_NetAddr::inet_readCfg(sNetAddr &inet, const QString &g)
{
    Cfg_Obj *cfg = mInetCfg;
    QString str = cfg->readCfg("ip", "", g).toString();
    qstrcpy(inet.ip, str.toStdString().c_str());
    str = cfg->readCfg("gw", "", g).toString();
    qstrcpy(inet.gw, str.toStdString().c_str());

    str = cfg->readCfg("mask", "", g).toString();
    qstrcpy(inet.mask, str.toStdString().c_str());
    str = cfg->readCfg("dns", "", g).toString();
    qstrcpy(inet.dns, str.toStdString().c_str());
    inet.en = cfg->readCfg("en", 0, g).toInt();
    inet.mode = cfg->readCfg("mode", 0, g).toInt();
    inet.prefixLen = cfg->readCfg("prefixLen", 0, g).toInt();
}

void App_NetAddr::inet_writeCfg(sNetAddr &inet, const QString &g)
{
    Cfg_Obj *cfg = mInetCfg;
    cfg->writeCfg("en", inet.en, g);
    cfg->writeCfg("ip", inet.ip, g);
    cfg->writeCfg("gw", inet.gw, g);
    cfg->writeCfg("dns", inet.dns, g);
    cfg->writeCfg("mask", inet.mask, g);
    cfg->writeCfg("mode", inet.mode, g);
    cfg->writeCfg("prefixLen", inet.prefixLen, g);
}


void App_NetAddr::inet_setInterface()
{
    if(!inet_isRun) {
        inet_isRun = true;
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
        QTimer::singleShot(455,this,SLOT(inet_setInterfaceSlot()));
#endif
        QTimer::singleShot(755,this,SLOT(inet_updateInterface()));
    }
}

void App_NetAddr::inet_setInterfaceSlot()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    if(strlen(net->name)) {
        if(net->inet.mode) {
            inet_dhcp(net->name);
        } else {
            inet_setIpV4();
        }
    }

    inet_isRun = false;
}

void App_NetAddr::inet_setIpV4()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    QString fn = net->name;
    QString ip = net->inet.ip;
    QString mask = net->inet.mask;
    QString cmd = "ifconfig %1 %2 netmask %3";
    QString str = cmd.arg(fn, ip, mask);
    system(str.toStdString().c_str());
    inet_writeCfg(net->inet, "IPV4");
}

void App_NetAddr::inet_dhcp(const QString &n)
{
    QString cmd = "dhclient " + n;
    system(cmd.toStdString().c_str());
}

void App_NetAddr::inet_updateInterface()
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
