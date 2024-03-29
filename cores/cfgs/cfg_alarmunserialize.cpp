/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_alarmunserialize.h"

Cfg_AlarmUnserialize::Cfg_AlarmUnserialize()
{

}


void Cfg_AlarmUnserialize::unAlarmUnit(uchar id, sAlarmUnit &unit, cfg::_sAlarmIt &it)
{
    unit.size = it.size;
    unit.en[id] = it.en;
    unit.hda[id] = it.hda;
    unit.min[id] = it.min;
    unit.max[id] = it.max;
    unit.crMin[id] = it.crMin;
    unit.crMax[id] = it.crMax;
    unit.rated[id] = it.rated;
    unit.peakMax[id] = it.peakMax;
    unit.peakStamp[id] = it.peakStamp;
    unit.reserve[0][id] = it.reserve[0];
}

void Cfg_AlarmUnserialize::unRelayUnit(uchar id, sRelayUnit &unit, cfg::_sRelayIt &it)
{
    unit.size = it.size;
    unit.cnt[id] = it.cnt;
    unit.maxCnt[id] = it.maxCnt;
    unit.lifeEn[id] = it.lifeEn;
    unit.disabled[id] = it.disabled;
    unit.offAlarm[id] = it.offAlarm;
    unit.powerUpDelay[id] = it.powerUpDelay;
    unit.resetDelay[id] = it.resetDelay;

    unit.timingEn[id] = it.timingEn;
    unit.overrunOff[id] = it.overrunOff;
    unit.timingOn[id][0] = unit.timingOff[id][0] = 0;
    qstrcpy(unit.timingOn[id], it.timingOn);
    qstrcpy(unit.timingOff[id], it.timingOff);
    unit.reserve[0][id] = it.reserve[0];
}

void Cfg_AlarmUnserialize::unObjData(uchar id, sObjData &data, cfg::_sObjData &obj)
{
    //qstrcpy(data.name[id], obj.name);
    data.hdaEle[id] = obj.hdaEle;
    unAlarmUnit(id, data.vol, obj.vol);
    unAlarmUnit(id, data.cur, obj.cur);
    unAlarmUnit(id, data.pow, obj.pow);
    unRelayUnit(id, data.relay, obj.relay);
    data.reserve[0][id] = obj.reserve[0];
}

void Cfg_AlarmUnserialize::unEnvData(uchar id, sEnvData &data, cfg::_sEnvData &obj)
{
    unAlarmUnit(id, data.tem, obj.tem);
    unAlarmUnit(id, data.hum, obj.hum);
    data.reserve[0][id] = obj.reserve[0];
}

void Cfg_AlarmUnserialize::unDevData(sDevData *data, cfg::_sDevData *obj)
{
    uchar size = obj->lineSize;
    for(int i=0; i< size; ++i) unObjData(i, data->line, obj->line[i]);
    data->line.relay.size = 0;

    size = obj->loopSize;
    for(int i=0; i< size; ++i) unObjData(i, data->loop, obj->loop[i]);

    size = obj->groupSize;
    for(int i=0; i< size; ++i) unObjData(i, data->group, obj->group[i]);

    size = obj->dualSize;
    for(int i=0; i<size; ++i) unObjData(i, data->dual, obj->dual[i]);
    data->dual.vol.size = data->dual.cur.size = data->dual.pow.size = 0;

    size = obj->cabLineSize;
    for(int i=0; i<size; ++i) unObjData(i, data->cabLine, obj->cabLine[i]);
    data->cabLine.vol.size = data->cabLine.cur.size = data->cabLine.pow.size = 0;

    size = obj->cabLoopSize;
    for(int i=0; i<size; ++i) unObjData(i, data->cabLoop, obj->cabLoop[i]);
    data->cabLoop.vol.size = data->cabLoop.cur.size = data->cabLoop.pow.size = 0;

    size = obj->outputSize;
    for(int i=0; i< size; ++i) unObjData(i, data->output, obj->output[i]);
    data->output.vol.size = 0;

    size = obj->envSize;
    for(int i=0; i< size; ++i) unEnvData(i, data->env, obj->env[i]);
    //data->env.hum.size = data->env.tem.size = size;

    data->tg = obj->tg;
    //data->info = obj->info;
    //cm::dataPacket()->login = obj->login;
}

void Cfg_AlarmUnserialize::unSequence()
{
    cfg::_sDevData *dev = getDev();
    unDevData(cm::masterDev(), dev);
}
