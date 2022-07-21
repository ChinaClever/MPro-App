/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_rwunserialize.h"

Cfg_RwUnserialize::Cfg_RwUnserialize(QObject *parent) : Cfg_RwFill{parent}
{

}


void Cfg_RwUnserialize::unAlarmUnit(uchar id, sAlarmUnit &unit, cfg::_sAlarmIt &it)
{
    unit.en[id] = it.en;
    unit.min[id] = it.min;
    unit.max[id] = it.max;
    unit.crMin[id] = it.crMin;
    unit.crMax[id] = it.crMax;
    unit.rated[id] = it.rated;
}

void Cfg_RwUnserialize::unRelayUnit(uchar id, sRelayUnit &unit, cfg::_sRelayIt &it)
{
    unit.en[id] = it.en;
    unit.mode[id] = it.state;
    unit.delay[id] = it.delay;
    unit.resTime[id] = it.resTime;
}

void Cfg_RwUnserialize::unObjData(uchar id, sObjData &data, cfg::_sObjData &obj)
{
    qstrcpy(data.name[id], obj.name);
    unAlarmUnit(id, data.vol, obj.vol);
    unAlarmUnit(id, data.cur, obj.cur);
    unAlarmUnit(id, data.pow, obj.pow);
    unRelayUnit(id, data.relay, obj.relay);
}

void Cfg_RwUnserialize::unEnvData(uchar id, sEnvData &data, cfg::_sEnvData &obj)
{
    unAlarmUnit(id, data.tem, obj.tem);
    unAlarmUnit(id, data.hum, obj.hum);
}

void Cfg_RwUnserialize::unDevData(sDevData *data, cfg::_sDevData *obj)
{
    uchar size = obj->lineSize;
    for(int i=0; i< size; ++i) unObjData(i, data->line, obj->line[i]);
    data->line.relay.size = 0;

    size = obj->loopSize;
    for(int i=0; i< size; ++i) unObjData(i, data->loop, obj->loop[i]);

    size = obj->groupSize;
    for(int i=0; i< size; ++i) unObjData(i, data->group, obj->group[i]);

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

void Cfg_RwUnserialize::unSequence()
{
    cfg::_sDevData *dev = getDev();
    unDevData(cm::masterDev(), dev);
}
