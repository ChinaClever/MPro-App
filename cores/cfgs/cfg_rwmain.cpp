/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_rwmain.h"

Cfg_RwMain::Cfg_RwMain()
{
    readSettings();
}

Cfg_RwMain *Cfg_RwMain::bulid()
{
    static Cfg_RwMain* sington = nullptr;
    if(sington == nullptr) {
        sington = new Cfg_RwMain();
    }
    return sington;
}

void Cfg_RwMain::writeAlarmDefault()
{
    QString cmd = "rm -f %1";
    system(cmd.arg(CFG_ALARM_DF).toLatin1().data());

    cmd = "cp -rf %1 %2";
    cmd = cmd.arg(CFG_ALARM_FN, CFG_ALARM_DF);
    system(cmd.toLatin1().data());
}

bool Cfg_RwMain::readSettings()
{
    bool ret = readParam(CFG_PARAM_FN);
    if(!ret) ret = readParamDefault();
    if(!ret) initialParam();

    ret = readAlarm(CFG_ALARM_FN);
    if(!ret) ret = readAlarmDefault();
    if(!ret) ret = initialData();
    readCfgParams();

    return ret;
}

