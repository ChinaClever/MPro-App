/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "daemons.h"

static void initSystem()
{
    system("cmd_fb enable /dev/fb0");
    system("cmd_fb display /dev/fb0");
    system("chmod +x -R /usr/data/clever/awtk/release/bin/");
    system("echo 3 > /proc/sys/vm/drop_caches"); system("sync");
    system("rm /usr/data/clever/awtk/release/assets/default/raw/images/xx/qrcode.png");
    //system("mount -t nfs 192.168.1.117:/home/lzy/work/nfs /usr/data/nfs");

    QString cmd = "ln -s /usr/data/clever/cfg/qrcode.png ";
    cmd += "/usr/data/clever/awtk/release/assets/default/raw/images/xx/qrcode.png";
    system(cmd.toLocal8Bit().data());
}

static QString mac_random()
{
    QList<uchar> vs; int k=0;
    QString fmd = "EF:%1:%2:%3:%4:%5";
    for(int i=0; i<6; ++i) vs << QRandomGenerator::global()->bounded(256);
    QString mac = fmd.arg(vs.at(k++), 2, 16, QLatin1Char('0'))
            .arg(vs.at(k++), 2, 16, QLatin1Char('0'))
            .arg(vs.at(k++), 2, 16, QLatin1Char('0'))
            .arg(vs.at(k++), 2, 16, QLatin1Char('0'))
            .arg(vs.at(k++), 2, 16, QLatin1Char('0'));
    return mac;
}

static void init_netWork()
{
    QString mac = "00:00:00:00:00:01";
    QString fn =  "/usr/data/clever/cfg/mac.ini";
    if(QFile::exists(fn)) {
        QFile file(fn);
        if(file.open(QIODevice::ReadOnly)) {
            QByteArray array = file.readAll().replace("\n", "");
            if(array.size() == mac.size()) {
                if(mac != array) mac = array;
                else mac = mac_random();
            } else qDebug() << "mac error" << array;
        }
    } else {
        mac = mac_random();
        //system("touch /usr/data/clever/cfg/mac.ini");
    }

    system("ip link set eth0 down");
    if(QFile::exists("netcfg")) {
        QString cmd = QString("./netcfg -w %1 eth0").arg(mac);
        system(cmd.toStdString().c_str()); qDebug() << cmd;
    } else {
        QString cmd = "ip link set eth0 address " +mac;
        system(cmd.toStdString().c_str()); qDebug() << cmd;
    }

    system("ip link set eth0 up");
    system("ip link set eth0 multicast on");
    //system("ip a flush dev eth0"); //　清掉所有IP地址
    system("ifconfig eth0 192.168.1.99 netmask 255.255.255.0");
    system("route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0");
    //system("dhclient");
}

static void createDirectory()
{
    system("mkdir -p /tmp/updater");
    system("mkdir -p /usr/data/upload");
    system("mkdir -p /usr/data/download");
    system("mkdir -p /usr/data/etc/ssl");
    system("mkdir -p /usr/data/etc/ssh");
    system("mkdir -p /usr/data/etc/snmp");
    system("mkdir -p /usr/data/clever/bin");
    system("mkdir -p /usr/data/clever/app");
    system("mkdir -p /usr/data/clever/cfg");
    system("mkdir -p /usr/data/clever/doc");
    system("mkdir -p /usr/data/clever/awtk");
    system("mkdir -p /usr/data/clever/certs");
    system("mkdir -p /usr/data/clever/outlet");
    system("mkdir -p /usr/data/clever/drivers");
    //system("touch /usr/data/etc/snmp/snmpd.conf");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    createDirectory();
    init_netWork();
    initSystem();

    Daemons::bulid();
#endif
    return a.exec();
}
