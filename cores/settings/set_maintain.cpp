/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_maintain.h"

Set_Maintain::Set_Maintain()
{

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

    return zip;
}

bool Set_Maintain::batchRestore(const QString &fn)
{
    return profileRestore(fn);
}

bool Set_Maintain::profileRestore(const QString &fn)
{
    bool ret = true; if(QFile::exists(fn)) {
        QString cmd = "unzip -o %1 -d /";
        cm::execute(cmd.arg(fn));
    } else ret = false;
    return ret;
}
