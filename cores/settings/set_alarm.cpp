/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_alarm.h"
#include "cascade_slave.h"

Set_Alarm::Set_Alarm()
{
}

bool Set_Alarm::setAlarm(const sSetAlarmUnit &unit)
{
    bool ret = true;
    if(unit.index.addr) {
        ret = Cascade_Core::bulid()->masterSetAlarm(unit);
    } else {
        if(unit.index.type) ret = setAlarmIt(unit);
        else ret = setTgAlarmUnit(unit);
        Set_readWrite::bulid()->writeSettings();
    }
    return ret;
}

bool Set_Alarm::setTgAlarmUnit(const sSetAlarmUnit &unit)
{
    bool ret = true;
    sTgUnit *it = getTgAlarmUnit(unit.index);
    if(it) {
        it->min = unit.min;
        it->max = unit.max;
        it->crMax = unit.crMax;
        it->crMin = unit.crMin;
        it->rated = unit.rated;
    } else {qDebug() << Q_FUNC_INFO; ret = false; }

    return ret;
}

bool Set_Alarm::setAlarmIt(const sSetAlarmUnit &unit)
{
    bool ret = true;
    sAlarmUnit *it = getAlarmUnit(unit.index);
    if(it) {
        uchar id = unit.index.id;
        if(id) setAlarmUnit(id--, it, unit);
        else for(int i=0; i<it->size; ++i) setAlarmUnit(i, it, unit);
    } else {qDebug() << Q_FUNC_INFO; ret = false; }

    return ret;
}

void Set_Alarm::setAlarmUnit(int id, sAlarmUnit *it, const sSetAlarmUnit &unit)
{
    it->min[id] = unit.min;
    it->max[id] = unit.max;
    it->rated[id] = unit.rated;
    it->crMin[id] = unit.crMin;
    it->crMax[id] = unit.crMax;
}
