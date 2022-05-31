/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_alarm.h"
#include "cascade_core.h"

Set_Alarm::Set_Alarm()
{
}


bool Set_Alarm::setAlarm(sDataItem &unit)
{
    bool ret = true;
    if(unit.rw) {
        ret = upIndexValue(unit);
        Set_ReadWrite::bulid()->writeSettings();
    }
    return ret;
}

