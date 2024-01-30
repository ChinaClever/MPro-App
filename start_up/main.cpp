/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "daemons.h"

static void createDirectory()
{
    system("mkdir -p /tmp/script");
    system("mkdir -p /tmp/download");
    system("mkdir -p /usr/data/upload");
    system("mkdir -p /usr/data/etc/ssl");
    system("mkdir -p /usr/data/etc/ssh");
    system("mkdir -p /usr/data/etc/snmp");
    system("mkdir -p /tmp/updater/ota_apps");
    system("mkdir -p /usr/data/pdu/bin");
    system("mkdir -p /usr/data/pdu/app");
    system("mkdir -p /usr/data/pdu/cfg");
    system("mkdir -p /usr/data/pdu/doc");
    system("mkdir -p /usr/data/pdu/awtk");
    system("mkdir -p /usr/data/pdu/certs");
    system("mkdir -p /usr/data/pdu/outlet");
    system("mkdir -p /usr/data/pdu/drivers");
}

static void initSystem()
{
    system("cmd_fb enable /dev/fb0");
    system("cmd_fb display /dev/fb0");
    system("rm -rf /usr/data/upload");
    system("chmod 775 -R /usr/data/pdu/bin");
    system("chmod 775 -R /usr/data/pdu/app");
    system("chmod 664 -R /usr/data/pdu/cfg");
    system("chmod 664 /usr/data/pdu/ver.ini");
    system("rm /usr/data/etc/snmp/snmpd.conf");
    //system("echo 3 > /proc/sys/vm/drop_caches");
    //system("mount -t nfs 192.168.1.117:/home/lzy/work/nfs /usr/data/nfs");
    system("rm /usr/data/pdu/awtk/release/assets/default/raw/images/xx/qrcode.png");

    //system("rm /usr/data/pdu/web/favicon.ico");
    //system("rm /usr/data/pdu/web/include/images/logo.png");
    QString cmd = "ln -sf /usr/data/pdu/cfg/logo.png ";
    cmd += "/usr/data/pdu/web/include/images/logo.png";
    system(cmd.toLocal8Bit().data());

    cmd = "ln -sf /usr/data/pdu/cfg/qrcode.png ";
    cmd += "/usr/data/pdu/awtk/release/assets/default/raw/images/xx/qrcode.png";
    system(cmd.toLocal8Bit().data()); createDirectory(); system("sync");
}

static void init_netWork()
{
    QString mac, deMac = "00:00:00:00:00:01";
    QString fn =  "/usr/data/pdu/cfg/mac.conf";
    if(QFile::exists(fn)) { QFile file(fn);
        if(file.open(QIODevice::ReadOnly)) mac = file.readAll();
        file.close(); if(!mac.contains(deMac)) {
            system("ip link set eth0 down");
            QString cmd = "ip link set eth0 address " + mac.mid(0, 17);
            system(cmd.toStdString().c_str()); qDebug() << cmd;
        }
    } //else system("echo '00:00:00:00:00:01' > /usr/data/pdu/cfg/mac.conf");

    system("ip link set eth0 up");
    //system("ip a flush dev eth0"); //　清掉所有IP地址
    system("ip link set eth0 multicast on");
    system("ifconfig eth0 192.168.1.163 netmask 255.255.255.0");
    system("route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0");
    //system("dhclient");
}


static void init_mem()
{
    QDir dir("/usr/data/pdu"); if(!dir.exists()) {
        system("mv /usr/data/clever /usr/data/pdu");
        system("ln -sf /usr/data/pdu /usr/data/clever");
    } system("echo 3 >/proc/sys/vm/drop_caches");
}


static void start_init()
{
    int cnt = 0; init_mem();
    //system("mount -o remount,rw /usr/data/");
    system("mv /tmp/messages /tmp/kernel_messages");
    QFile file("/usr/data/pdu/cfg/proc_cnt.conf");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        cnt = file.readAll().toInt(); file.seek(0);
        QString str = QString::number(cnt+1);
        file.write(str.toStdString().c_str());
    } file.close(); initSystem(); system("sync");
    if(cnt < 150) system("chmod 775 -R /usr/data/pdu &");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    start_init();
    init_netWork();
    Daemons::bulid();
#endif
    return a.exec();
}
