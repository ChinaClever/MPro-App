/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_rwmain.h"
#include "cfg_obj.h"

Cfg_RwMain::Cfg_RwMain(QObject *parent)
    : Cfg_ReadParam{parent}
{
    QTimer::singleShot(15,this,SLOT(readSettings()));
}

Cfg_RwMain *Cfg_RwMain::bulid(QObject *parent)
{
    static Cfg_RwMain* sington = nullptr;
    if(sington == nullptr) {
        sington = new Cfg_RwMain(parent);
    }
    return sington;
}

void Cfg_RwMain::writeDefault()
{
    QString cmd = "rm -f %1";
    system(cmd.arg(CFG_DEFAULT_FN).toLatin1().data());

    cmd = "cp -rf %1 %2";
    cmd = cmd.arg(CFG_DATA_FN, CFG_DEFAULT_FN);
    system(cmd.toLatin1().data());
}

bool Cfg_RwMain::readSettings()
{
    mThread->init(this, SLOT(run()));
    bool ret = readSetting(CFG_DATA_FN);
    if(!ret) ret = readDefault();
    if(!ret) ret = initialData();
    readCfgParams();

    return ret;
}

