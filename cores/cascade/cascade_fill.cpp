/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_fill.h"

Cascade_Fill::Cascade_Fill(QObject *parent) : Cascade_Master{parent}
{

}

void Cascade_Fill::upAlarmUnit(uchar id, sAlarmUnit &unit, c_sAlarmUnit &it)
{
    it.size = unit.size;
    it.en = unit.en[id];
    it.hda = unit.hda[id];
    it.value = unit.value[id];
    it.rated = unit.rated[id];

    it.min = unit.min[id];
    it.max = unit.max[id];

    it.crMin = unit.crMin[id];
    it.crMax = unit.crMax[id];
    it.peakMax = unit.peakMax[id];
    it.peakStamp = unit.peakStamp[id];
    it.reserve[0] = unit.reserve[0][id];
}

void Cascade_Fill::upRelayUnit(uchar id, sRelayUnit &unit, c_sRelayUnit &it)
{
    it.size = unit.size;
    it.sw = unit.sw[id];
    it.cnt = unit.cnt[id];
    it.lifeEn = unit.lifeEn[id];
    it.maxCnt = unit.maxCnt[id];
    it.disabled = unit.disabled[id];
    it.offAlarm = unit.offAlarm[id];
    it.powerUpDelay = unit.powerUpDelay[id];
    it.overrunOff = unit.overrunOff[id];
    it.timingEn = unit.timingEn[id];
    it.timingOn[0] = it.timingOff[0] = 0;
    qstrcpy(it.timingOn, unit.timingOn[id]);
    qstrcpy(it.timingOff, unit.timingOff[id]);
    it.reserve[0] = unit.reserve[0][id];
}

void Cascade_Fill::upObjData(uchar id, sObjData &data, c_sObjData &obj)
{
    obj.id = id; obj.name[0] = 0;
    qstrcpy(obj.name, data.name[id]);
    upAlarmUnit(id, data.vol, obj.vol);
    upAlarmUnit(id, data.cur, obj.cur);
    upAlarmUnit(id, data.pow, obj.pow);
    upRelayUnit(id, data.relay, obj.relay);

    obj.pf = data.pf[id];
    obj.ele = data.ele[id];
    obj.hdaEle = data.hdaEle[id];
    obj.lineVol = data.lineVol[id];
    obj.activePow = data.artPow[id];
    obj.reactivePow = data.reactivePow[id];
    obj.reserve[0] = data.reserve[0][id];
}

void Cascade_Fill::upEnvData(uchar id, sEnvData &data, c_sEnvData &obj)
{
    obj.type_index = id;
    obj.isInsert = data.isInsert[id];
    upAlarmUnit(id, data.tem, obj.tem);
    upAlarmUnit(id, data.hum, obj.hum);

    obj.wind = data.wind[id];
    obj.door = data.door[id];
    obj.water = data.water[id];
    obj.smoke = data.smoke[id];
    obj.reserve[0] = data.reserve[0][id];
}

void Cascade_Fill::upDevData(uchar addr, sDevData *data, c_sDevData *obj)
{
    obj->id = addr; obj->cfg = data->cfg; //obj->uut = data->uut;
    uchar size = obj->lineSize = data->line.size;
    for(int i=0; i< size; ++i) upObjData(i, data->line, obj->line[i]);

    size = obj->loopSize = data->loop.size;
    for(int i=0; i< size; ++i) upObjData(i, data->loop, obj->loop[i]);

    size = obj->groupSize = data->group.size;
    for(int i=0; i< size; ++i) upObjData(i, data->group, obj->group[i]);

    size = obj->dualSize = data->dual.size;
    for(int i=0; i< size; ++i) upObjData(i, data->dual, obj->dual[i]);

    size = obj->outputSize = data->output.size;
    for(int i=0; i< size; ++i) upObjData(i, data->output, obj->output[i]);

    obj->envSize = data->env.size; if(!size) size = SENOR_NUM;
    for(int i=0; i< size; ++i) upEnvData(i, data->env, obj->env[i]);

    obj->dtc = data->dtc;
    obj->rtu = data->rtu;
    obj->tg = data->tg;
    obj->lps = data->lps;
    obj->dc = data->dc;
    obj->hz = data->hz;
    obj->proc = data->proc;
    obj->reserve[0] = data->reserve[0];
}

void Cascade_Fill::fillData(uchar addr)
{
    c_sDevData *dev = getDev();
    sDevData *data = cm::masterDev();
    upDevData(addr, data, dev);
}
