/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_core.h"

Cfg_Core::Cfg_Core()
{
    readSettings();
}

Cfg_Core *Cfg_Core::bulid()
{
    static Cfg_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Cfg_Core();
    }
    return sington;
}

void Cfg_Core::writeAlarmDefault()
{
    QString cmd = "rm -f %1";
    system(cmd.arg(CFG_ALARM_DF).toLatin1().data());

    cmd = "cp -rf %1 %2";
    cmd = cmd.arg(CFG_ALARM_FN, CFG_ALARM_DF);
    system(cmd.toLatin1().data());
}

bool Cfg_Core::readSettings()
{
    bool ret = readAlarm(CFG_ALARM_FN);
    if(!ret) ret = readAlarm(CFG_ALARM_DF);
    if(!ret) ret = initialData();

    return ret;
}

