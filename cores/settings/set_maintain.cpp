/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_maintain.h"

Set_Maintain::Set_Maintain()
{

}

static void dev_restart()
{
    cm::mdelay(1000);
    system("sync");
    system("reboot");
}


bool Set_Maintain::syscmd(int fc)
{
    bool ret=true; switch (fc) {
    case 1: m_thread = new std::thread(dev_restart); m_thread->detach(); break;
    case 2: factoryRestore(); break;
    default: ret = false; cout << fc; break;
    }
    return ret;
}

void Set_Maintain::factoryRestore()
{
    //sEventItem it;
    //it.event_type = QStringLiteral("恢复");
    //it.event_content = QStringLiteral("恢复出厂设置");
    //Log_Core::bulid()->append(it);

    QStringList fns;
    QString dir = "/usr/data/clever/cfg/";
    Cfg_Core::bulid()->devParamRestoreFactory();
    system("chmod 777 /usr/data/clever/cfg/*");
    system("echo '1' > /usr/data/clever/cfg/factoryRestore");
    fns << "logs.db" << "alarm.cfg" << "proc_cnt.ini";
    fns << "cfg.ini" << "inet.ini";

    foreach (const auto &fn, fns) {
        QString fmd = "rm -rf %1%2";
        QString cmd = fmd.arg(dir, fn);
        system(cmd.toLocal8Bit().data());
    } //system("sync"); system("reboot");
    m_thread = new std::thread(dev_restart);
    m_thread->detach(); //子线程与主线程分离
}

QString Set_Maintain::backups(int fc)
{
    QString res; switch (fc) {
    case 1: res = profileBackup(); break;
    case 2: res = batchBackup(); break;
    default: cout << fc; break;
    }

    return res;
}

bool Set_Maintain::restores(int fc, const QVariant &v)
{
    bool ret=true; switch (fc) {
    case 1: ret = profileRestore(v.toString()); break;
    case 2: ret = batchRestore(v.toString()); break;
    default: ret = false; cout << fc; break;
    }

    return ret;
}

QString Set_Maintain::profileBackup()
{
    QString fmd = "zip -vr %1 %2";
    QString src = "/usr/data/clever/cfg";
    QString dst = "/tmp/download/cfg.zip";
    QString cmd = fmd.arg(dst, src);
    system("rm -rf /tmp/download/*");
    system(cmd.toLocal8Bit().data());

    sEventItem it; if(cm::cn()) {
        it.event_type = QStringLiteral("备份");
        it.event_content = QStringLiteral("设备设置备份");
    } else {
        it.event_type = "backups";
        it.event_content = "device settings backup";
    } Log_Core::bulid()->append(it);

    return dst;
}

QString Set_Maintain::batchBackup()
{
    QStringList fns;
    QString dir = "usr/data/clever/cfg/";
    fns << "mac.ini" << "inet.ini" << "devParam.ini";
    fns << "logs.db" << "qrcode.png" << "proc_cnt.ini";
    fns << "uuid.conf" << "sn.conf";

    QString zip = profileBackup();
    foreach (const auto &fn, fns) {
        QString fmd = "zip -d %1 %2%3";
        QString cmd = fmd.arg(zip, dir, fn);
        system(cmd.toLocal8Bit().data());
    }

    sEventItem it; if(cm::cn()) {
        it.event_type = QStringLiteral("备份");
        it.event_content = QStringLiteral("批量设置备份");
    } else {
        it.event_type = "backups";
        it.event_content = "batch settings backup";
    } Log_Core::bulid()->append(it);

    return zip;
}

bool Set_Maintain::batchRestore(const QString &fn)
{
    sEventItem it; if(cm::cn()) {
        it.event_type = QStringLiteral("恢复");
        it.event_content = QStringLiteral("批量设置恢复");
    } else {
        it.event_type = "restore";
        it.event_content = "batch settings recovery";
    } Log_Core::bulid()->append(it);
    return restory(fn);
}

bool Set_Maintain::profileRestore(const QString &fn)
{
    sEventItem it; if(cm::cn()) {
        it.event_type = QStringLiteral("恢复");
        it.event_content = QStringLiteral("设备配置恢复");
    } else {
        it.event_type = "restore";
        it.event_content = "device configuration recovery";
    } Log_Core::bulid()->append(it);
    return restory(fn);
}

bool Set_Maintain::restory(const QString &fn)
{
    bool ret = true; if(QFile::exists(fn)) {
        QString fmd = "unzip -o %1 -d /";
        QString cmd = fmd.arg(fn); qDebug() << cmd;
        system("chmod 775 /usr/data/clever/cfg/*");
        system(cmd.toLatin1().data()); //cm::mdelay(50);
        system("chmod 775 /usr/data/clever/cfg/*");
        cm::execute("rm -rf " + fn);
        m_thread = new std::thread(dev_restart);
        m_thread->detach(); //子线程与主线程分离
        //cm::mdelay(1650); //system("reboot");
    } else ret = false;
    return ret;
}
