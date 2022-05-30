/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_rwunserialize.h"

Set_RwUnserialize::Set_RwUnserialize(QObject *parent) : Set_RwFill{parent}
{

}


void Set_RwUnserialize::unAlarmUnit(uchar id, sAlarmUnit &unit, set::_sAlarmIt &it)
{
    unit.min[id] = it.min;
    unit.max[id] = it.max;
    unit.crMin[id] = it.crMin;
    unit.crMax[id] = it.crMax;
    unit.rated[id] = it.rated;
}

void Set_RwUnserialize::unRelayUnit(uchar id, sRelayUnit &unit, set::_sRelayIt &it)
{
    unit.sw[id] = it.sw;
    unit.mode[id] = it.state;
    unit.delay[id] = it.delay;
}

void Set_RwUnserialize::unObjData(uchar id, sObjData &data, set::_sObjData &obj)
{
    qstrcpy(data.name[id], obj.name);
    unAlarmUnit(id, data.vol, obj.vol);
    unAlarmUnit(id, data.cur, obj.cur);
    unAlarmUnit(id, data.pow, obj.pow);
    unRelayUnit(id, data.relay, obj.relay);
}

void Set_RwUnserialize::unEnvData(uchar id, sEnvData &data, set::_sEnvData &obj)
{
    unAlarmUnit(id, data.tem, obj.tem);
    unAlarmUnit(id, data.hum, obj.hum);
}

void Set_RwUnserialize::unDevData(sDevData *data, set::_sDevData *obj)
{
    uchar size = obj->lineSize;
    for(int i=0; i< size; ++i) unObjData(i, data->line, obj->line[i]);
    data->line.relay.size = 0;

    size = obj->loopSize;
    for(int i=0; i< size; ++i) unObjData(i, data->loop, obj->loop[i]);

    size = obj->outputSize;
    for(int i=0; i< size; ++i) unObjData(i, data->output, obj->output[i]);
    data->output.vol.size = 0;

    size = obj->envSize;
    for(int i=0; i< size; ++i) unEnvData(i, data->env, obj->env[i]);
    //data->env.hum.size = data->env.tem.size = size;

    data->tg = obj->tg;
    data->info = obj->info;
    cm::dataPacket()->login = obj->login;
}

void Set_RwUnserialize::unSequence()
{
    set::_sDevData *dev = getDev();
    unDevData(cm::masterDev(), dev);
}
