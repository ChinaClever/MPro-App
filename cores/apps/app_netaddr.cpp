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
    if(net->inet.dhcp || (net->inet6.dhcp && net->inet6.en)) system("udhcpc &");
    else system("killall dhcpd");

    if(!strlen(net->inet.ip)) {
        sNetAddr *inet = &net->inet;
        inet->en = 1; inet->dhcp = 0;
        qstrcpy(inet->gw, "192.168.1.1");
        qstrcpy(inet->ip, "192.168.1.163");
        qstrcpy(inet->mask, "255.255.255.0");
        inet_writeCfg(*inet, "IPV4");
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

void App_NetAddr::inet_writeCfg(const sNetAddr &inet, const QString &g)
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
        if(net->inet.dhcp || net->inet6.dhcp) t = 9500;
        QTimer::singleShot(87,this,&App_NetAddr::inet_setInterfaceSlot);
        QTimer::singleShot(1543,this,&App_NetAddr::inet_setInterfaceSlot);
        QTimer::singleShot(1654,this,&App_NetAddr::inet_updateInterface);
        if(t) QTimer::singleShot(t+587,this,&App_NetAddr::inet_setInterfaceSlot);
        if(t) QTimer::singleShot(2*t+587,this,&App_NetAddr::inet_setInterfaceSlot);
    }
}

void App_NetAddr::inet_setInterfaceSlot()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    inet_setIpV4(); //inet_saveCfg();

    if(net->inet6.en) inet_setIpV6();
    else mInetCfg->writeCfg("en", 0, "IPV6");
    cm::mdelay(1); inet_isRun = false;
    //system("arp -a &");
}

void App_NetAddr::inet_setIpV4()
{
    sNetInterface *net = &(cm::dataPacket()->net);
    if(net->inet.dhcp) {
        //net->inet.ip[0] = 0;
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
            if(QFile::exists("/usr/data/pdu/bin/netcfg")) {
                cmd = "netcfg -g %1 eth0"; str = cmd.arg(gw);
            } else {
                cmd = "ip route replace default via %1 dev %2";
                str = cmd.arg(gw, fn);
            } //qDebug() << str;
            system(str.toStdString().c_str());
        }

        if(dns.size() || dns2.size()) {
            cmd = "netcfg -d \"%1 %2\"";
            str = cmd.arg(dns, dns2); //qDebug() << str;
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
        if(QFile::exists("/usr/data/pdu/bin/netcfg")) {
            cmd = "netcfg -i %1/%2 eth0";
            str = cmd.arg(ip).arg(mask);
        } else {
            cmd = "ip -6 addr add %1/%2 dev %3";
            str = cmd.arg(ip).arg(mask).arg(fn);
        } system(str.toStdString().c_str()); // qDebug() << str;

        if(gw.size()) {
            if(QFile::exists("/usr/data/pdu/bin/netcfg")) {
                cmd = "netcfg -g %1 eth0";
                str = cmd.arg(gw);
            } else {
                cmd = "ip -6 route replace default via %1 dev %2";
                str = cmd.arg(gw, fn);
            } system(str.toStdString().c_str()); //qDebug() << str;
        }

        if(dns.size() || dns2.size()) {
            cmd = "netcfg -d \"%1 %2\"";
            str = cmd.arg(dns, dns2); //qDebug() << str;
            system(str.toStdString().c_str());
        }
    }
}

void App_NetAddr::inet_saveCfg(int fc, const sNetInterface &net)
{
    //sNetInterface *net = &(cm::dataPacket()->net);
    if(fc) inet_writeCfg(net.inet6, "IPV6");
    else inet_writeCfg(net.inet, "IPV4");
    //QTimer::singleShot(1200,this,&App_NetAddr::inet_updateInterface);
}

void App_NetAddr::inet_dnsCfg()
{
    if(!QFile::exists("/tmp/resolv.conf")) return ;
    sNetInterface *net = &(cm::dataPacket()->net);
    QString str = cm::execute("cat /tmp/resolv.conf");
    QStringList res, lst = str.split("\n");
    foreach (auto s, lst) res << s.remove("nameserver ").remove(" # eth0");
    if(res.isEmpty()) return; // else qDebug() << str << res;

    net->inet.dns[0] = 0;
    net->inet.dns2[0] = 0;
    net->inet6.dns[0] = 0;
    net->inet6.dns2[0] = 0;

    if(res.size() > 1) {
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

int App_NetAddr::inet_dhcpUpdate()
{
    static int t = 30; t *= 2;
    sNetInterface *net = &(cm::dataPacket()->net);
    if(net->inet.dhcp || net->inet6.dhcp) {
        if(mCnt < 15*60) t = 1;
        else if(mCnt < 30*60) t = 2;
        else if(mCnt < 60*60) t = 3;
        else if(mCnt < 4*60*60) t = 4;
        else if(mCnt < 12*60*60) t = 6;
        else if(mCnt < 24*60*60) t = 8;
        else if(mCnt < 48*60*60) t = 10;
        else if(mCnt < 72*60*60) t = 15;
        else t = 30;
    }

    if((!net->inet.dhcp) && net->inet6.en && net->inet6.dhcp){
        static QString str = mInetCfg->readCfg("ip", "", "IPV4").toString();
        if(str != net->inet.ip) {
            inet_readCfg(net->inet, "IPV4");
            inet_setIpV4();
        }
    }
    return t;
}

QString App_NetAddr::getIpv4GatewayAddress()
{
    QString gw = cm::dataPacket()->net.inet.gw;
    QString cmd = "ip route show dev eth0 | awk '/default/ {print $3}'";
    QStringList res = cm::execute(cmd).split(" ");
    if(res.size() > 4) gw = res.at(2);
    //cout << res << gw;
    return gw;
}

QString App_NetAddr::getIpv6GatewayAddress()
{
    QString gw = cm::dataPacket()->net.inet6.gw;
    QString cmd = "ip -6 route show dev eth0 | awk '/default/ {print $3}'";
    QString res = cm::execute(cmd).split("\n", QString::SkipEmptyParts).last();
    QStringList lst = res.split(" "); if(lst.size() > 4) gw = lst.at(0);
    //cout << cm::execute(cmd).split("\n") << res << lst <<  gw;
    return gw;
}

void App_NetAddr::inet_updateInterface()
{
    int t = inet_dhcpUpdate(); mCnt += t;
    QTimer::singleShot(t*1000,this,&App_NetAddr::inet_updateInterface);
    sNetInterface *net = &(cm::dataPacket()->net); inet_dnsCfg(); QString str; int k=0;    
    QList<QNetworkInterface>list = QNetworkInterface::allInterfaces();//获取所有网络接口信息
    if(net->inet6.dhcp && net->inet6.en)inet_setIpV4();
    foreach(QNetworkInterface interface, list) {  //便利每一个接口信息
        if(interface.name() != "eth0") continue;
        qstrcpy(net->name, interface.name().toLatin1().constData());//设备名称
        qstrcpy(net->mac, interface.hardwareAddress().toLatin1().constData());//获取并输出mac地址
        qstrcpy(net->inet.gw, getIpv4GatewayAddress().toLatin1().constData());
        qstrcpy(net->inet6.gw, getIpv6GatewayAddress().toLatin1().constData());
        QList<QNetworkAddressEntry>entryList=interface.addressEntries();//获取ip地址和子网掩码和广播地址
        foreach(QNetworkAddressEntry entry, entryList) {//便利ip条目列表
            QHostAddress hostIp = entry.ip();
            if(hostIp != QHostAddress(QHostAddress::LocalHost)) {
                switch (hostIp.protocol()) {
                case QAbstractSocket::IPv4Protocol:
                    memset(net->inet.ip, 0, NAME_SIZE);
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
