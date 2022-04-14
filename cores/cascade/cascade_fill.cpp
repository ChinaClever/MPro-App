#include "cascade_fill.h"

Cascade_Fill::Cascade_Fill(QObject *parent) : Cascade_Master{parent}
{

}

void Cascade_Fill::upAlarmUnit(uchar id, sAlarmUnit &unit, c_sAlarmUnit &it)
{
    it.value = unit.value[id];
    it.rated = unit.rated[id];

    it.min = unit.min[id];
    it.max = unit.max[id];

    it.crMin = unit.crMin[id];
    it.crMax = unit.crMax[id];
}

void Cascade_Fill::upRelayUnit(uchar id, sRelayUnit &unit, c_sRelayUnit &it)
{
    it.sw = unit.sw[id];
    it.state = unit.mode[id];
    it.delay = unit.delay[id];
}

void Cascade_Fill::upObjData(uchar id, sObjData &data, c_sObjData &obj)
{
    obj.id = id;
    qstrcpy(obj.name, data.name[id]);
    upAlarmUnit(id, data.vol, obj.vol);
    upAlarmUnit(id, data.cur, obj.cur);
    upAlarmUnit(id, data.pow, obj.pow);
    upRelayUnit(id, data.relay, obj.relay);

    obj.pf = data.pf[id];
    obj.ele = data.ele[id];
    obj.activePow = data.artPow[id];
    obj.reactivePow = data.reactivePow[id];
}

void Cascade_Fill::upEnvData(uchar id, sEnvData &data, c_sEnvData &obj)
{
    obj.type_index = id;
    upAlarmUnit(id, data.tem, obj.tem);
    upAlarmUnit(id, data.hum, obj.hum);

    obj.door = data.door[id];
    obj.water = data.water[id];
    obj.smoke = data.smoke[id];
}

void Cascade_Fill::upDevData(uchar addr, sDevData *data, c_sDevData *obj)
{
    obj->id = addr; obj->info = data->info; obj->uut = data->uut;
    uchar size = obj->lineSize = data->line.size;
    for(int i=0; i< size; ++i) upObjData(i, data->line, obj->line[i]);

    size = obj->loopSize = data->loop.size;
    for(int i=0; i< size; ++i) upObjData(i, data->loop, obj->loop[i]);

    size = obj->outputSize = data->output.size;
    for(int i=0; i< size; ++i) upObjData(i, data->output, obj->output[i]);

    size = obj->envSize = data->env.size;
    for(int i=0; i< size; ++i) upEnvData(i, data->env, obj->env[i]);

    obj->rtuCount = data->rtuCount;
    obj->tg = data->tg;
    obj->lps = data->lps;
    obj->dc = data->dc;
    obj->hz = data->hz;
    obj->br = data->br;
}

void Cascade_Fill::fillData(uchar addr)
{
    c_sDevData *dev = getDev();
    sDevData *data = cm::masterDev();
    data->hz = 66;
    upDevData(addr, data, dev);
}
