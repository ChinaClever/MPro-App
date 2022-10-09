/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "daemons.h"

__attribute__((constructor)) void startup()
{
    std::system("./proc_log daemon_startup");
    //printf("Constructor is called.\n");
}

__attribute__((destructor)) void app_exit()
{
    std::system("./proc_log daemon_exit");
    //printf("destructor is called.\n");
}


static void initSystem()
{
    system("echo 3 > /proc/sys/vm/drop_caches");
    system("ifconfig eth0 up"); //system("dhclient");
    system("ifconfig eth0 192.168.1.99 netmask 255.255.255.0");
    system("route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0");
    //system("sync"); system("echo 3 > /proc/sys/vm/drop_caches");
    //system("mount -t nfs 192.168.1.130:/home/lzy/work/nfs /usr/data/nfs");
}

static void startSnmpd()
{
    QString custom = "/usr/data";
    QString fn = "/etc/snmpd.conf";
    QString cmd = "snmpd -f -Lo -C -c ";
    bool ret = ProcRuning::isRun("snmpd"); if(ret) return ;
    if(QFile::exists(custom + fn)) {
        cmd += custom + fn + " &";
    } else if(QFile::exists(fn)) {
        cmd +=  fn + " &";
    } else {
        cmd.clear();
    }

    qDebug() << cmd;
    if(cmd.size()) system(cmd.toLatin1().data());
    else qDebug() << "Error: start snmpd error";
}

static void createDirectory()
{
    system("mkdir -p /usr/data/etc/ssl");
    system("mkdir -p /usr/data/etc/ssh");
    system("mkdir -p /usr/data/etc/snmp");
    system("mkdir -p /usr/data/clever/app");
    system("mkdir -p /usr/data/clever/cfg");
    system("mkdir -p /usr/data/clever/awtk");
    system("mkdir -p /usr/data/clever/certs");
    system("mkdir -p /usr/data/clever/upload");
    system("mkdir -p /usr/data/clever/drivers");
    system("mkdir -p /usr/data/clever/download");
    system("touch /usr/data/etc/snmp/snmpd.conf");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    createDirectory();
    initSystem();
    startSnmpd();

    // Daemons::bulid();
#endif
    return a.exec();
}
