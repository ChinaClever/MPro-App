/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "procstart.h"

ProcStart::ProcStart()
{

}

void ProcStart::proc_start(const QString &app, const QString &arg)
{
    QString path = "/usr/data/clever/app/";
    QString fn = path + app;
    if(QFile::exists(fn)) {
        QString cmd = fn;
        if(arg.size()) cmd += " " + arg;
        cmd += " &"; system(cmd.toLatin1().data());
    } else qDebug() << "proc start err:" << fn;
}

void ProcStart::proc_log(const QString &arg)
{
    proc_start("proc_log", arg);
}

void ProcStart::startCore()
{
    proc_log("core_startup");
    proc_start("cores");
}

void ProcStart::startLcd()
{    
    QString path = "/usr/data/clever/awtk/release/bin/";
    QString fn = path + "demo"; proc_log("awtk_startup");
    if(QFile::exists(fn)) {
        QString cmd = fn; cmd += " &";
        system(cmd.toLatin1().data());
        proc_log("awtk_startup");
    } else qDebug() << "proc start err:" << fn;
}
