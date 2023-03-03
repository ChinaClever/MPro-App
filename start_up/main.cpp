/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "daemons.h"

static void createDirectory()
{
    system("mkdir -p /tmp/updater");
    system("mkdir -p /tmp/download");
    system("mkdir -p /usr/data/upload");
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
}

static void initSystem()
{
    system("chmod 777 -R /usr/data/clever");
    system("chmod 777 -R /usr/data/etc/snmp");
    system("echo 3 > /proc/sys/vm/drop_caches"); system("sync");
    //system("mount -t nfs 192.168.1.117:/home/lzy/work/nfs /usr/data/nfs");
    system("rm /usr/data/clever/awtk/release/assets/default/raw/images/xx/qrcode.png");

    //system("rm /usr/data/clever/web/favicon.ico");
    system("rm /usr/data/clever/web/include/images/logo.png");
    QString cmd = "ln -s /usr/data/clever/cfg/logo.png ";
    cmd += "/usr/data/clever/web/include/images/logo.png";
    system(cmd.toLocal8Bit().data());

    cmd = "ln -s /usr/data/clever/cfg/qrcode.png ";
    cmd += "/usr/data/clever/awtk/release/assets/default/raw/images/xx/qrcode.png";
    system(cmd.toLocal8Bit().data()); createDirectory();
}

static void init_netWork()
{
    QString mac, deMac = "00:00:00:00:00:01";
    QString fn =  "/usr/data/clever/cfg/mac.ini";
    if(QFile::exists(fn)) { QFile file(fn);
        if(file.open(QIODevice::ReadOnly)) mac = file.readAll();
    } else system("echo '00:00:00:00:00:01' > /usr/data/clever/cfg/mac.ini");

    if(!mac.contains(deMac)) {
        system("ip link set eth0 down");
        QString cmd = "ip link set eth0 address " + mac.mid(0, 17);
        system(cmd.toStdString().c_str()); qDebug() << cmd;
    }

    system("ip link set eth0 up");
    //system("ip a flush dev eth0"); //　清掉所有IP地址
    system("ip link set eth0 multicast on");
    system("ifconfig eth0 192.168.1.99 netmask 255.255.255.0");
    system("route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0");
    //system("dhclient");
}


static void start_init()
{
    int min = 350; int max = 10*1024;
    //system("mount -o remount,rw /usr/data/");
    QString fn = "/usr/data/clever/cfg/proc_log.txt";
    QFile file(fn); int size = file.size();
    if(size < min) initSystem();
    else if(size > max) file.remove();
    system("cmd_fb enable /dev/fb0");
    system("cmd_fb display /dev/fb0");
    system("rm /tmp/messages");
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
