/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_unserialize.h"

Cascade_Unserialize::Cascade_Unserialize(QObject *parent) : Cascade_Updater{parent}
{

}


void Cascade_Unserialize::unAlarmUnit(uchar id, sAlarmUnit &unit, c_sAlarmUnit &it)
{
    unit.size = it.size;
    unit.en[id] = it.en;
    unit.hda[id] = it.hda;
    unit.min[id] = it.min;
    unit.max[id] = it.max;
    unit.value[id] = it.value;
    unit.rated[id] = it.rated;
    unit.crMin[id] = it.crMin;
    unit.crMax[id] = it.crMax;
    unit.peakMax[id] = it.peakMax;
    unit.peakStamp[id] = it.peakStamp;
    unit.reserve[0][id] = it.reserve[0];
}

void Cascade_Unserialize::unRelayUnit(uchar id, sRelayUnit &unit, c_sRelayUnit &it)
{
    unit.size = it.size;
    unit.sw[id] = it.sw;
    unit.cnt[id] = it.cnt;
    unit.lifeEn[id] = it.lifeEn;
    unit.maxCnt[id] = it.maxCnt;
    unit.disabled[id] = it.disabled;
    unit.offAlarm[id] = it.offAlarm;
    unit.powerUpDelay[id] = it.powerUpDelay;
    unit.overrunOff[id] = it.overrunOff;
    unit.timingEn[id] = it.timingEn;
    unit.timingOn[id][0] = unit.timingOff[id][0] = 0;
    qstrcpy(unit.timingOn[id], it.timingOn);
    qstrcpy(unit.timingOff[id], it.timingOff);
    unit.reserve[0][id] = it.reserve[0];
}

void Cascade_Unserialize::unObjData(uchar id, sObjData &data, c_sObjData &obj)
{
    data.name[id][0] = 0;
    qstrcpy(data.name[id], obj.name);
    unAlarmUnit(id, data.vol, obj.vol);
    unAlarmUnit(id, data.cur, obj.cur);
    unAlarmUnit(id, data.pow, obj.pow);
    unRelayUnit(id, data.relay, obj.relay);

    data.pf[id] = obj.pf;
    data.ele[id] = obj.ele;
    data.hdaEle[id] = obj.hdaEle;
    data.lineVol[id] = obj.lineVol;
    data.artPow[id] = obj.activePow;
    data.reactivePow[id] = obj.reactivePow;
    data.reserve[0][id] = obj.reserve[0];
}

void Cascade_Unserialize::unEnvData(uchar id, sEnvData &data, c_sEnvData &obj)
{
    obj.type_index = id;
    data.isInsert[id] = obj.isInsert;
    unAlarmUnit(id, data.tem, obj.tem);
    unAlarmUnit(id, data.hum, obj.hum);

    data.wind[id] = obj.wind;
    data.door[id] = obj.door;
    data.water[id] = obj.water;
    data.smoke[id] = obj.smoke;
    data.reserve[0][id] = obj.reserve[0];
}

void Cascade_Unserialize::unDevSize(int devSpec, uchar size, sObjData &data)
{
    int num = size;
    if((1==devSpec) || (devSpec == 3)) num = 0;
    data.size = num;
    data.vol.size = num;
    data.cur.size = num;
    data.pow.size = num;

    if(devSpec < 3) size = 0;
    data.relay.size = size;
}

void Cascade_Unserialize::unDevData(sDevData *data, c_sDevData *obj)
{
    data->cfg = obj->cfg; //int ds = data->cfg.param.devSpec; //data->uut = obj->uut;
    uchar size = data->line.size = obj->lineSize; //unDevSize(ds, size, data->line);
    for(int i=0; i< size; ++i) unObjData(i, data->line, obj->line[i]);
    data->line.relay.size = 0;

    size = data->loop.size = obj->loopSize; //unDevSize(ds, size, data->loop);
    for(int i=0; i< size; ++i) unObjData(i, data->loop, obj->loop[i]);

    size = data->group.size = obj->groupSize; //unDevSize(ds, size, data->group);
    for(int i=0; i< size; ++i) unObjData(i, data->group, obj->group[i]);
    //if(4 != data->cfg.param.devSpec) data->group.relay.size = 0;
    data->group.vol.size = data->group.cur.size = 0;

    size = data->dual.size = obj->dualSize; //unDevSize(ds, size, data->dual);
    for(int i=0; i< size; ++i) unObjData(i, data->dual, obj->dual[i]);
    data->dual.vol.size = data->dual.cur.size = 0;

    size = data->output.size = obj->outputSize; //unDevSize(ds, size, data->output);
    for(int i=0; i< size; ++i) unObjData(i, data->output, obj->output[i]);
    data->output.vol.size = 0;

    data->env.size = obj->envSize; if(!size) size = SENOR_NUM;
    for(int i=0; i< size; ++i) unEnvData(i, data->env, obj->env[i]);
    data->env.hum.size = data->env.tem.size = data->env.size;

    data->dtc = obj->dtc;
    data->rtu = obj->rtu;
    data->tg = obj->tg;
    data->lps = obj->lps;
    data->dc = obj->dc;
    data->hz = obj->hz;
    data->proc = obj->proc;
    data->reserve[0] = obj->reserve[0];
}

bool Cascade_Unserialize::unSequence(uchar addr)
{
    bool ret = true;
    c_sDevData *dev = getDev();
    if(dev->id == addr) {
        unDevData(cm::devData(addr), dev);
    } else {ret = false; qDebug() << "Error: Cascade Unserialize" << addr << dev->id;}

    return ret;
}
