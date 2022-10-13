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


/***
 * 在/usr/data/目录下，建立 启动脚本 start_up.sh
 * 必须得先 执行升级命令 cp -rf /usr/data/updater/clever/ *  /usr/data/clever/
 * 再清空升级目录　rm -rf /usr/data/updater/clever/ *
 */

static void initSystem()
{
    system("chmod 777 /usr/data/clever/app/*");
    system("chmod 777 /usr/data/clever/awtk/release/bin/*");
    system("ifconfig eth0 192.168.1.99 netmask 255.255.255.0");
    system("route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0");
    system("ln -s /usr/data/clever/awtk/release/bin/demo /usr/data/clever/app/awtk");
    system("echo 3 > /proc/sys/vm/drop_caches"); system("sync");
    system("ifconfig eth0 up"); system("dhclient");
    //system("mount -t nfs 192.168.1.130:/home/lzy/work/nfs /usr/data/nfs");
}

static void startSnmpd()
{
    QString fn = "snmpd.conf";
    QString cmd = "snmpd -f -Lo -C -c ";
    bool ret = ProcStart::proc_isRun("snmpd"); if(ret) return ;

    QString custom = "/usr/data/clever/cfg/";
    if(QFile::exists(custom + fn)) {
        cmd += custom + fn + " &";
    } else if(QFile::exists("/etc/"+fn)) {
        cmd += "/etc/"+ fn + " &";
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
    system("mkdir -p /usr/data/clever/bin");
    system("mkdir -p /usr/data/clever/app");
    system("mkdir -p /usr/data/clever/cfg");
    system("mkdir -p /usr/data/clever/awtk");
    system("mkdir -p /usr/data/clever/certs");
    system("mkdir -p /usr/data/clever/outlet");
    system("mkdir -p /usr/data/clever/upload");
    system("mkdir -p /usr/data/clever/drivers");
    system("mkdir -p /usr/data/clever/download");
    system("mkdir -p /usr/data/updater/clever");
    //system("touch /usr/data/etc/snmp/snmpd.conf");
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
