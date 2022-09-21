/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_alarmfill.h"

Cfg_AlarmFill::Cfg_AlarmFill()
{

}


void Cfg_AlarmFill::upAlarmUnit(uchar id, sAlarmUnit &unit, cfg::_sAlarmIt &it)
{
    it.en = unit.en[id];
    it.rated = unit.rated[id];

    it.min = unit.min[id];
    it.max = unit.max[id];

    it.crMin = unit.crMin[id];
    it.crMax = unit.crMax[id];    
    it.reserve = unit.reserve[id];
}

void Cfg_AlarmFill::upRelayUnit(uchar id, sRelayUnit &unit, cfg::_sRelayIt &it)
{
    it.en = unit.en[id];
    it.reserve = unit.reserve[id];
    it.offAlarm = unit.offAlarm[id];
    it.powerUpDelay = unit.powerUpDelay[id];
    it.resetDelay = unit.resetDelay[id];
    it.overrunOff = unit.overrunOff[id];
    it.timingEn = unit.timingEn[id];
    qstrcpy(it.timingOn, unit.timingOn[id]);
    qstrcpy(it.timingOff, unit.timingOff[id]);
}

void Cfg_AlarmFill::upObjData(uchar id, sObjData &data, cfg::_sObjData &obj)
{
    qstrcpy(obj.name, data.name[id]);
    upAlarmUnit(id, data.vol, obj.vol);
    upAlarmUnit(id, data.cur, obj.cur);
    upAlarmUnit(id, data.pow, obj.pow);
    upRelayUnit(id, data.relay, obj.relay);
}

void Cfg_AlarmFill::upEnvData(uchar id, sEnvData &data, cfg::_sEnvData &obj)
{
    upAlarmUnit(id, data.tem, obj.tem);
    upAlarmUnit(id, data.hum, obj.hum);
}

void Cfg_AlarmFill::upDevData(sDevData *data, cfg::_sDevData *obj)
{
    uchar size = obj->lineSize = LINE_NUM;
    for(int i=0; i<size; ++i) upObjData(i, data->line, obj->line[i]);

    size = obj->loopSize = LOOP_NUM;
    for(int i=0; i<size; ++i) upObjData(i, data->loop, obj->loop[i]);

    size = obj->groupSize = GROUP_NUM;
    for(int i=0; i<size; ++i) upObjData(i, data->group, obj->group[i]);

    size = obj->dualSize = OUTPUT_NUM;
    for(int i=0; i<size; ++i) upObjData(i, data->dual, obj->dual[i]);

    size = obj->outputSize = OUTPUT_NUM;
    for(int i=0; i<size; ++i) upObjData(i, data->output, obj->output[i]);

    size = obj->envSize = SENOR_NUM;
    for(int i=0; i<size; ++i) upEnvData(i, data->env, obj->env[i]);

    obj->tg = data->tg;
    //obj->info = data->info;
    //obj->login = cm::dataPacket()->login;
    obj->tg.vol.value = obj->tg.cur.value = obj->tg.pow.value = 0;
    obj->tg.pf = obj->tg.artPow = 0;
}

void Cfg_AlarmFill::fillData()
{
    cfg::_sDevData *dev = getDev();
    sDevData *data = cm::masterDev();
    upDevData(data, dev);
}
