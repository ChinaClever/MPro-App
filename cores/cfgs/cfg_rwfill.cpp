/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_rwfill.h"

Cfg_RwFill::Cfg_RwFill(QObject *parent) : Cfg_RwObj{parent}
{

}


void Cfg_RwFill::upAlarmUnit(uchar id, sAlarmUnit &unit, cfg::_sAlarmIt &it)
{
    it.en = unit.en[id];
    it.rated = unit.rated[id];

    it.min = unit.min[id];
    it.max = unit.max[id];

    it.crMin = unit.crMin[id];
    it.crMax = unit.crMax[id];
}

void Cfg_RwFill::upRelayUnit(uchar id, sRelayUnit &unit, cfg::_sRelayIt &it)
{
    it.en = unit.en[id];
    it.state = unit.mode[id];
    it.delay = unit.delay[id];
    it.resTime = unit.resTime[id];
}

void Cfg_RwFill::upObjData(uchar id, sObjData &data, cfg::_sObjData &obj)
{
    qstrcpy(obj.name, data.name[id]);
    upAlarmUnit(id, data.vol, obj.vol);
    upAlarmUnit(id, data.cur, obj.cur);
    upAlarmUnit(id, data.pow, obj.pow);
    upRelayUnit(id, data.relay, obj.relay);
}

void Cfg_RwFill::upEnvData(uchar id, sEnvData &data, cfg::_sEnvData &obj)
{
    upAlarmUnit(id, data.tem, obj.tem);
    upAlarmUnit(id, data.hum, obj.hum);
}

void Cfg_RwFill::upDevData(sDevData *data, cfg::_sDevData *obj)
{
    uchar size = data->line.size;
    for(int i=0; i<size; ++i) upObjData(i, data->line, obj->line[i]);

    size = data->loop.size;
    for(int i=0; i<size; ++i) upObjData(i, data->loop, obj->loop[i]);

    size = data->output.size;
    for(int i=0; i<size; ++i) upObjData(i, data->output, obj->output[i]);

    size = data->env.size;
    for(int i=0; i<size; ++i) upEnvData(i, data->env, obj->env[i]);

    obj->tg = data->tg;
    obj->info = data->info;

    //obj->login = cm::dataPacket()->login;
    obj->tg.vol.value = obj->tg.cur.value = obj->tg.pow.value = 0;
    obj->tg.pf = obj->tg.artPow = 0;
}

void Cfg_RwFill::fillData()
{
    cfg::_sDevData *dev = getDev();
    sDevData *data = cm::masterDev();
    upDevData(data, dev);
}
