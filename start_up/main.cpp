/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "daemons.h"

__attribute__((constructor)) void startup()
{
    //printf("Constructor is called.\n");
}

__attribute__((destructor)) void app_exit()
{
    //std::system("./proc_log daemon_exit");
    printf("destructor is called.\n");
}


static void initSystem()
{
    system("chmod +x -R /usr/data/clever/awtk/release/bin/");
    system("echo 3 > /proc/sys/vm/drop_caches"); system("sync");
    //system("mount -t nfs 192.168.1.117:/home/lzy/work/nfs /usr/data/nfs");
}

static void init_netWork()
{
    QString mac = "00:00:00:00:00:01";
    QString fn =  "/usr/data/clever/cfg/mac.ini";
    if(QFile::exists(fn)) {
        QFile file(fn);
        if(file.open(QIODevice::ReadOnly)) {
            QByteArray array=file.readAll();
            if(array.size() == 17) mac = array;
            else qDebug() << "mac error" << array;
        }

    } else {
        system("touch /usr/data/clever/cfg/mac.ini");
    }

    //system("ip link set eth0 down");
    if(QFile::exists("netcfg")) {
        QString cmd = QString("netcfg -w %1 eth0").arg(mac);
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
    system("mkdir -p /usr/data/etc/ssl");
    system("mkdir -p /usr/data/etc/ssh");
    system("mkdir -p /usr/data/etc/snmp");
    system("mkdir -p /usr/data/clever/bin");
    system("mkdir -p /usr/data/clever/app");
    system("mkdir -p /usr/data/clever/cfg");
    system("mkdir -p /usr/data/clever/awtk");
    system("mkdir -p /usr/data/clever/certs");
    //system("mkdir -p /usr/data/clever/outlet");
    system("mkdir -p /usr/data/clever/upload");
    system("mkdir -p /usr/data/clever/drivers");
    system("mkdir -p /usr/data/clever/download");
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
