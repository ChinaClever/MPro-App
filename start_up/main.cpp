/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "daemons.h"

static void createDirectory()
{
    system("mkdir -p /tmp/download");
    system("mkdir -p /usr/data/upload");
    system("mkdir -p /usr/data/etc/ssl");
    system("mkdir -p /usr/data/etc/ssh");
    system("mkdir -p /usr/data/etc/snmp");
    system("mkdir -p /tmp/updater/ota_apps");
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
    system("cmd_fb enable /dev/fb0");
    system("cmd_fb display /dev/fb0");
    system("rm -rf /usr/data/upload");
    system("chmod 755 -R /usr/data/clever/bin");
    system("chmod 755 -R /usr/data/clever/app");
    system("chmod 777 /usr/data/clever/ver.ini");
    system("rm /usr/data/etc/snmp/snmpd.conf");
    system("echo 3 > /proc/sys/vm/drop_caches");
    //system("mount -t nfs 192.168.1.117:/home/lzy/work/nfs /usr/data/nfs");
    system("rm /usr/data/clever/awtk/release/assets/default/raw/images/xx/qrcode.png");

    //system("rm /usr/data/clever/web/favicon.ico");
    system("rm /usr/data/clever/web/include/images/logo.png");
    QString cmd = "ln -s /usr/data/clever/cfg/logo.png ";
    cmd += "/usr/data/clever/web/include/images/logo.png";
    system(cmd.toLocal8Bit().data());

    cmd = "ln -s /usr/data/clever/cfg/qrcode.png ";
    cmd += "/usr/data/clever/awtk/release/assets/default/raw/images/xx/qrcode.png";
    system(cmd.toLocal8Bit().data()); createDirectory(); system("sync");
}

static void init_netWork()
{
    QString mac, deMac = "00:00:00:00:00:01";
    QString fn =  "/usr/data/clever/cfg/mac.ini";
    if(QFile::exists(fn)) { QFile file(fn);
        if(file.open(QIODevice::ReadOnly)) mac = file.readAll();
        file.close(); if(!mac.contains(deMac)) {
            system("ip link set eth0 down");
            QString cmd = "ip link set eth0 address " + mac.mid(0, 17);
            system(cmd.toStdString().c_str()); qDebug() << cmd;
        }
    } //else system("echo '00:00:00:00:00:01' > /usr/data/clever/cfg/mac.ini");

    system("ip link set eth0 up");
    //system("ip a flush dev eth0"); //　清掉所有IP地址
    system("ip link set eth0 multicast on");
    system("ifconfig eth0 192.168.1.99 netmask 255.255.255.0");
    system("route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0");
    //system("dhclient");
}


static void init_mem()
{
    //system("echo 38912 > /proc/sys/vm/min_free_kbytes");
    system("echo 800 > /proc/sys/vm/vfs_cache_pressure");
    system("echo 0 > /proc/sys/vm/dirty_background_bytes");
    system("echo 1 > /proc/sys/vm/dirty_background_ratio");
    system("echo 100 > /proc/sys/vm/dirty_writeback_centisecs");
    system("echo 600 > /proc/sys/vm/dirty_expire_centisecs");
    system("echo 0 > /proc/sys/vm/dirty_bytes");
    system("echo 1 > /proc/sys/vm/panic_on_oom");
    system("echo 2 > /proc/sys/vm/dirty_ratio");
    system("echo 3 >/proc/sys/vm/drop_caches");
}


static void start_init()
{
    int cnt = 0; init_mem();
    //system("mount -o remount,rw /usr/data/");
    system("mv /tmp/messages /tmp/kernel_messages");
    QFile file("/usr/data/clever/cfg/proc_cnt.ini");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        cnt = file.readAll().toInt(); file.seek(0);
        QString str = QString::number(cnt+1);
        file.write(str.toStdString().c_str());
    } file.close(); initSystem(); system("sync");
    if(cnt < 500) system("chmod 755 -R /usr/data/clever");
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
