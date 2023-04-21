/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_netaddr.h"
#include <QNetworkInterface>

App_NetAddr::App_NetAddr(QObject *parent)
    : App_SensorBox{parent}
{
    QTimer::singleShot(1,this,&App_NetAddr::inet_initFunSlot);
}

void App_NetAddr::inet_initFunSlot()
{
    QString fn = Cfg_Obj::pathOfCfg("inet.ini");
    mInetCfg = new Cfg_Obj(fn, this);

    sNetInterface *net = &(cm::dataPacket()->net);
    inet_readCfg(net->inet, "IPV4"); net->inet.en = 1;
    inet_readCfg(net->inet6, "IPV6"); qstrcpy(net->name, "eth0");
    if(net->inet.dhcp || net->inet6.dhcp) system("udhcpc &");

    if(!strlen(net->inet.ip)) {
        sNetAddr *inet = &net->inet;
        inet->en = 1; inet->dhcp = 0;
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
    str = cfg->readCfg("dns2", "", g).toString();
    qstrcpy(inet.dns2, str.toStdString().c_str());

    inet.en = cfg->readCfg("en", 0, g).toInt();
    inet.dhcp = cfg->readCfg("dhcp", 0, g).toInt();
    inet.prefixLen = cfg->readCfg("prefixLen", 0, g).toInt();
}

void App_NetAddr::inet_writeCfg(sNetAddr &inet, const QString &g)
{
    Cfg_Obj *cfg = mInetCfg;
    cfg->writeCfg("en", inet.en, g);
    cfg->writeCfg("ip", inet.ip, g);
    cfg->writeCfg("gw", inet.gw, g);
    cfg->writeCfg("dns", inet.dns, g);
    cfg->writeCfg("dns2", inet.dns2, g);
    cfg->writeCfg("mask", inet.mask, g);
    cfg->writeCfg("dhcp", inet.dhcp, g);
    cfg->writeCfg("prefixLen", inet.prefixLen, g);
}

void App_NetAddr::inet_setInterface()
{
    if(!inet_isRun) {
        inet_isRun = true; int t = 0;
        sNetInterface *net = &(cm::dataPacket()->net);
        if(net->inet.dhcp || net->inet6.dhcp) t = 6500;
        QTimer::singleShot(87,this,&App_NetAddr::inet_setInterfaceSlot);
        QTimer::singleShot(1234,this,&App_NetAddr::inet_updateInterface);
        if(t) QTimer::singleShot(t+587,this,&App_NetAddr::inet_setInterfaceSlot);
        if(t) QTimer::singleShot(t+1234,this,&App_NetAddr::inet_updateInterface);
    }
}

void App_NetAddr::inet_setInterfaceSlot()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    inet_setIpV4(); //inet_saveCfg();

    if(net->inet6.en) inet_setIpV6();
    else mInetCfg->writeCfg("en", 0, "IPV6");
    cm::mdelay(1); inet_isRun = false;
}

void App_NetAddr::inet_setIpV4()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    if(net->inet.dhcp) {
        net->inet.ip[0] = 0;
    } else {
        QString fn = net->name;
        QString ip = net->inet.ip;
        QString gw = net->inet.gw;
        QString mask = net->inet.mask;
        QString dns = net->inet.dns;
        QString dns2 = net->inet.dns2;
        QString cmd = "ifconfig %1 %2 netmask %3";
        QString str = cmd.arg(fn, ip, mask);
        system(str.toStdString().c_str());
        //qDebug() << str;

        if(gw.size()) {
            if(QFile::exists("netcfg")) {
                cmd = "netcfg -g %1 eth0";
                str = cmd.arg(gw);
            } else {
                cmd = "ip route replace default via %1 dev %2";
                str = cmd.arg(gw, fn);
            } qDebug() << str;
            system(str.toStdString().c_str());
        }

        if(dns.size() || dns2.size()) {
            cmd = "netcfg -d \"%1 %2\"";
            str = cmd.arg(dns, dns2); qDebug() << str;
            system(str.toStdString().c_str());
        }
    }
}

void App_NetAddr::inet_setIpV6()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    if(net->inet6.dhcp) {
        net->inet6.ip[0] = 0;
    } else {
        QString fn = net->name;
        QString ip = net->inet6.ip;
        QString gw = net->inet6.gw;
        QString dns = net->inet6.dns;
        QString dns2 = net->inet6.dns2;
        int mask = net->inet6.prefixLen;
        QString cmd, str;
        if(QFile::exists("netcfg")) {
            cmd = "netcfg -i %1/%2 eth0";
            str = cmd.arg(ip).arg(mask);
        } else {
            cmd = "ip -6 addr add %1/%2 dev %3";
            str = cmd.arg(ip).arg(mask).arg(fn);
        } qDebug() << str; system(str.toStdString().c_str());

        if(gw.size()) {
            if(QFile::exists("netcfg")) {
                cmd = "netcfg -g %1 eth0";
                str = cmd.arg(gw);
            } else {
                cmd = "ip -6 route replace default via %1 dev %2";
                str = cmd.arg(gw, fn);
            } qDebug() << str;
            system(str.toStdString().c_str());
        }

        if(dns.size() || dns2.size()) {
            cmd = "netcfg -d \"%1 %2\"";
            str = cmd.arg(dns, dns2); qDebug() << str;
            system(str.toStdString().c_str());
        }
    }
}

void App_NetAddr::inet_saveCfg(int fc)
{
    sNetInterface *net = &(cm::dataPacket()->net);
    if(fc) inet_writeCfg(net->inet6, "IPV6");
    else inet_writeCfg(net->inet, "IPV4");
}

void App_NetAddr::inet_dnsCfg()
{
    if(!QFile::exists("/tmp/resolv.conf")) return ;
    sNetInterface *net = &(cm::dataPacket()->net);
    QString str = cm::execute("cat /tmp/resolv.conf");
    if(str.isEmpty()) return; else str.remove(" # eth0").remove("\n");
    QStringList res = str.split("nameserver ");
    qDebug() << str << res;

    net->inet.dns[0] = 0;
    net->inet.dns2[0] = 0;
    net->inet6.dns[0] = 0;
    net->inet6.dns2[0] = 0;

    if(res.size() > 1) {
        str = res.takeFirst();
        str = res.takeFirst();
        qstrcpy(net->inet.dns, str.toLocal8Bit().data());
        if(!res.size()) return ;

        str = res.takeFirst();
        qstrcpy(net->inet.dns2, str.toLocal8Bit().data());
        if(!res.size()) return ;

        str = res.takeFirst();
        qstrcpy(net->inet6.dns, str.toLocal8Bit().data());
        if(!res.size()) return ;

        str = res.takeFirst();
        qstrcpy(net->inet6.dns2, str.toLocal8Bit().data());
        if(!res.size()) return ;
    }
}

void App_NetAddr::inet_updateInterface()
{
    sNetInterface *net = &(cm::dataPacket()->net); inet_dnsCfg(); QString str; int k=0;
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
                    if((0==net->inet6.en) || net->inet6.dhcp) {
                        qstrcpy(net->inet6.ip, hostIp.toString().remove("%eth0").toLatin1().constData()); //获取ip
                        qstrcpy(net->inet6.mask, entry.netmask().toString().toLatin1().constData()); //获取子网掩码
                        net->inet6.prefixLen = entry.prefixLength();//获取子网掩码
                    } str = hostIp.toString() + "/" + QString::number(entry.prefixLength());
                    if(k<3)qstrcpy(net->inet6.reserve[k++], str.remove("%eth0").toLatin1().constData());
                    break;

                default:
                    cout << net->name << net->mac << hostIp.toString() << entry.netmask().toString();
                    break;
                }
            }
        }
    }
}
