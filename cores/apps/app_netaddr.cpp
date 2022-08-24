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
    if(!strlen(net->name[0])) {
        sNetAddr *inet = &net->inet[0];
        inet->en = 1; inet->mode = 0;
        qstrcpy(inet->gw, "192.168.1.1");
        qstrcpy(inet->ip, "192.168.1.99");
        qstrcpy(inet->mask, "255.255.255.0");
        qstrcpy(net->name[0], "eth0");
    } net->inet[0].en = 1;
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
    for(int i=0; i<NET_NUM; ++i) {
        if(strlen(net->name[i])) {
            if(net->inet[i].mode) {
                dhcp(net->name[i]);
            } else {
                setIpV4(i);
            }
        }
    }

    isRun = false;
}

void App_NetAddr::setIpV4(int id)
{
    sNetInterface *net = &(cm::dataPacket()->net);
    QString fn = net->name[id];
    QString ip = net->inet->ip;
    QString mask = net->inet->mask;
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
    sNetInterface *net = &(cm::dataPacket()->net); int id = 0;
    QList<QNetworkInterface>list = QNetworkInterface::allInterfaces();//获取所有网络接口信息
    foreach(QNetworkInterface interface, list) {  //便利每一个接口信息
        if(interface.name() == "lo") continue;
        qstrcpy(net->name[id], interface.name().toLatin1().constData());//设备名称
        qstrcpy(net->mac[id], interface.hardwareAddress().toLatin1().constData());//获取并输出mac地址
        QList<QNetworkAddressEntry>entryList=interface.addressEntries();//获取ip地址和子网掩码和广播地址
        foreach(QNetworkAddressEntry entry, entryList) {//便利ip条目列表
            QHostAddress hostIp = entry.ip();
            if(hostIp != QHostAddress(QHostAddress::LocalHost)) {
                switch (hostIp.protocol()) {
                case QAbstractSocket::IPv4Protocol:
                    net->inet[id].prefixLen = entry.prefixLength();//获取子网掩码
                    qstrcpy(net->inet[id].ip, hostIp.toString().toLatin1().constData()); //获取ip
                    qstrcpy(net->inet[id].mask, entry.netmask().toString().toLatin1().constData()); //获取子网掩码
                    break;

                case QAbstractSocket::IPv6Protocol:
                    qstrcpy(net->inet6[id].ip, hostIp.toString().toLatin1().constData()); //获取ip
                    qstrcpy(net->inet6[id].mask, entry.netmask().toString().toLatin1().constData()); //获取子网掩码
                    net->inet6[id].prefixLen = entry.prefixLength();//获取子网掩码
                    break;
                default:
                    break;
                }
            }
        } if(id++) break;
    }
}
