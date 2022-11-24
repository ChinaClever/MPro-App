/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_diagnose.h"
#include "integr_jsonbuild.h"

Set_Diagnose::Set_Diagnose()
{

}

void Set_Diagnose::sys_cmd(const QString &cmd, const QString &fn)
{
    QString fmd = "echo '\n%1:' >> /usr/data/download/dia/%2";
    cm::execute(fmd.arg(cmd, fn));

    fmd = "%1 >> /usr/data/download/dia/%2";
    cm::execute(fmd.arg(cmd, fn));
}

void Set_Diagnose::diasCopy()
{
    QString dir = "/usr/data/download/dia";
    QString cmd = "mkdir -p " + dir; cm::execute(cmd);
    cmd = "rm -rf %1/*"; cm::execute(cmd.arg(dir));
    cmd = "mkdir -p " + dir +"/metadata";
    cm::execute(cmd.toLocal8Bit().data());
    Integr_JsonBuild::bulid()->saveJsons();

    QString fmd = "cp -af %1 " + dir +"/";
    cmd = fmd.arg("/usr/data/etc"); cm::execute(cmd);
    cmd = fmd.arg("/usr/data/clever/bin"); cm::execute(cmd);
    cmd = fmd.arg("/usr/data/clever/cfg"); cm::execute(cmd);
    cmd = fmd.arg("/usr/data/clever/certs"); cm::execute(cmd);
    cmd = fmd.arg("/usr/data/clever/ver.ini"); cm::execute(cmd);
}

void Set_Diagnose::diascmds()
{
    sys_cmd("cat /proc/cpuinfo", "cpu");
    sys_cmd("cat /proc/cpu-feature-override", "cpu");

    sys_cmd("df", "filesystems");
    sys_cmd("df -h /usr/data/", "filesystems");
    sys_cmd("cat /proc/mounts", "filesystems");

    sys_cmd("cat /proc/interrupts", "interrupts");
    sys_cmd("cat /proc/softirqs", "interrupts");

    sys_cmd("lsmod", "kernel-modules");
    sys_cmd("cat /proc/modules", "kernel-modules");

    sys_cmd("cat /proc/loadavg", "load-and-uptime");
    sys_cmd("cat /proc/uptime", "load-and-uptime");

    sys_cmd("free", "memory");
    sys_cmd("cat /proc/meminfo", "memory");
    sys_cmd("cat /proc/pagetypeinfo", "memory");
    sys_cmd("cat /proc/zoneinfo", "memory");
    sys_cmd("cat /proc/softirqs", "memory");
    sys_cmd("cat /proc/stat", "memory");

    sys_cmd("ifconfig", "network");
    sys_cmd("ip addr", "network");
    sys_cmd("ip neigh", "network");
    sys_cmd("ip rule", "network");
    sys_cmd("ip route show table all", "network");

    sys_cmd("ethtool eth0", "network");
    sys_cmd("ethtool -S eth0", "network");
    sys_cmd("cat /proc/net/dev", "network");
    sys_cmd("cat /proc/sys/net/ipv4/conf/all/forwarding", "network");
    sys_cmd("cat /proc/sys/net/ipv6/conf/all/forwarding", "network");

    sys_cmd("ps", "processes");
    sys_cmd("top -bmn1", "processes");
    sys_cmd("lsusb", "usb-devices");
    sys_cmd("dmesg | grep 'usb'", "usb-devices");
}


QString Set_Diagnose::diasZip()
{
    QString dir = "/usr/data/download";
    QString cmd = "zip -vr %1/dias.zip %1/dias";
    cm::execute(cmd.arg(dir));
    return dir+"/dias.zip";
}

QString Set_Diagnose::downDiagnostics()
{
    diasCopy();
    diascmds();
    return diasZip();
}

QVariant Set_Diagnose::net_diagnoseCfg(int fc)
{
    sNetDgsIt *it = &mNetDgs;
    QVariant res; switch (fc) {
    case 1: res = it->pingHost;  break;
    case 2: res = it->pingCnt;  break;
    case 3: res = cm::execute(QString("ping -c%1 %2").arg(it->pingCnt).arg(it->pingHost));  break;

    case 6: res = it->routeHost;  break;
    case 7: res = it->timeout; break;
    case 8: res = cm::execute(QString("traceroute -w %1 %2").arg(it->timeout).arg(it->routeHost));  break;
    default: cout << fc; break;
    }

    return res;
}

bool Set_Diagnose::net_diagnoseSet(int fc, const QVariant &v)
{
    sNetDgsIt *it = &mNetDgs;
    bool ret = true; switch (fc) {
    case 1: it->pingHost = v.toString(); break;
    case 2: it->pingCnt = v.toInt(); break;
    case 6: it->routeHost = v.toString();  break;
    case 7: it->timeout = v.toInt(); break;
    default: ret = false; cout << fc; break;
    }
    return ret;
}
