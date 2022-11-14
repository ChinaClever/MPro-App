/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_maintain.h"

Set_Maintain::Set_Maintain()
{

}

bool Set_Maintain::syscmd(int fc)
{
    bool ret=true; switch (fc) {
    case 1: ret = system("reboot"); break;
    case 2: ret = factoryRestore(); break;
    default: ret = false; cout << fc; break;
    }

    return ret;
}

bool Set_Maintain::factoryRestore()
{
    sEventItem it;
    it.type = QStringLiteral("恢复");
    it.content = QStringLiteral("恢复出厂设置");
    Log_Core::bulid()->append(it);

    QStringList fns;
    QString dir = "usr/data/clever/cfg/";
    fns << "logs.db" << "proc_log.txt";
    fns << "cfg.ini" << "inet.ini" << "alarm.cfg";

    foreach (const auto &fn, fns) {
        QString cmd = "rm -f %1%2";
        cm::execute(cmd.arg(dir, fn));
    } cm::mdelay(650);

    return true;
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
    QString cmd = "zip -vr %1 %2";
    QString src = "/usr/data/clever/cfg";
    QString dst = "/usr/data/clever/download/cfg.zip";
    cm::execute("rm -rf /usr/data/clever/download/*");
    cm::execute(cmd.arg(dst, src));

    sEventItem it;
    it.type = QStringLiteral("备份");
    it.content = QStringLiteral("设备设置备份");
    Log_Core::bulid()->append(it);

    return dst;
}

QString Set_Maintain::batchBackup()
{
    QStringList fns;
    QString dir = "usr/data/clever/cfg/";
    fns << "logs.db" << "proc_log.txt";
    fns << "mac.ini" << "inet.ini" << "devParam.ini";

    QString zip = profileBackup();
    foreach (const auto &fn, fns) {
        QString cmd = "zip -d %1 %2%3";
        cm::execute(cmd.arg(zip, dir, fn));
    }

    sEventItem it;
    it.type = QStringLiteral("备份");
    it.content = QStringLiteral("批量设置备份");
    Log_Core::bulid()->append(it);

    return zip;
}

bool Set_Maintain::batchRestore(const QString &fn)
{
    sEventItem it;
    it.type = QStringLiteral("恢复");
    it.content = QStringLiteral("批量设置恢复");
    Log_Core::bulid()->append(it);
    return restory(fn);
}

bool Set_Maintain::profileRestore(const QString &fn)
{
    sEventItem it;
    it.type = QStringLiteral("恢复");
    it.content = QStringLiteral("设备配置恢复");
    Log_Core::bulid()->append(it);
    return restory(fn);
}

bool Set_Maintain::restory(const QString &fn)
{
    bool ret = true; if(QFile::exists(fn)) {
        QString cmd = "unzip -o %1 -d /";
        cm::execute(cmd.arg(fn));
        cm::mdelay(650);
        system("reboot");
    } else ret = false;
    return ret;
}
