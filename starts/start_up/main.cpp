/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "start_app.h"

static void initSystem()
{
    system("ifconfig eth0 up");
    system("echo 3 > /proc/sys/vm/drop_caches");
    system("ifconfig eth0 192.168.1.99 netmask 255.255.255.0");
    system("route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0");
}

static void startSnmpd()
{
    QString custom = "/usr/data";
    QString fn = "/etc/snmpd.conf";
    QString cmd = "snmpd -f -Lo -C -c ";
    bool ret = App_Run::isRun("snmpd");
    if(ret) return ;
    if(QFile::exists(custom + fn)) {
        cmd += custom + fn + " &";
    } else if(QFile::exists(custom + fn)) {
        cmd +=  fn + " &";
    } else {
        cmd.clear();
    }

    qDebug() << cmd;
    if(cmd.size()) system(cmd.toLatin1().data());
    else qDebug() << "Error: start snmpd error";
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    initSystem();
    startSnmpd();

    QObject *p = a.parent();
    Start_App::bulid(p);
#endif
    return a.exec();
}
