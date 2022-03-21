#include "cascade_unserialize.h"

Cascade_Unserialize::Cascade_Unserialize(QObject *parent) : Cascade_Updater{parent}
{

}


void Cascade_Unserialize::unAlarmUnit(uchar id, sAlarmUnit &unit, c_sAlarmUnit &it)
{
    unit.value[id] = it.value;
    unit.rated[id] = it.rated;

    unit.min[id] = it.min;
    unit.max[id] = it.max;
    unit.crMin[id] = it.crMin;
    unit.crMax[id] = it.crMax;
}

void Cascade_Unserialize::unRelayUnit(uchar id, sRelayUnit &unit, c_sRelayUnit &it)
{
    unit.sw[id] = it.sw;
    unit.mode[id] = it.state;
    unit.delay[id] = it.delay;
}

void Cascade_Unserialize::unObjData(uchar id, sObjData &data, c_sObjData &obj)
{
    qstrcpy(data.name[id], obj.name);
    unAlarmUnit(id, data.vol, obj.vol);
    unAlarmUnit(id, data.cur, obj.cur);
    unAlarmUnit(id, data.pow, obj.pow);
    unRelayUnit(id, data.relay, obj.relay);

    data.pf[id] = obj.pf;
    data.ele[id] = obj.ele;
    data.artPow[id] = obj.activePow;
    data.reactivePow[id] = obj.reactivePow;
}

void Cascade_Unserialize::unEnvData(uchar id, sEnvData &data, c_sEnvData &obj)
{
    obj.type_index = id;
    unAlarmUnit(id, data.tem, obj.tem);
    unAlarmUnit(id, data.hum, obj.hum);

    data.door[id] = obj.door;
    data.water[id] = obj.water;
    data.smoke[id] = obj.smoke;
}

void Cascade_Unserialize::unDevSize(uchar size, sObjData &data)
{
    data.size = size;
    data.vol.size = size;
    data.cur.size = size;
    data.pow.size = size;
    data.relay.size = size;
}

void Cascade_Unserialize::unDevData(sDevData *data, c_sDevData *obj)
{
    data->info = obj->info; data->uut = obj->uut;
    uchar size = data->line.size = obj->lineSize; unDevSize(size, data->line);
    for(int i=0; i< size; ++i) unObjData(i, data->line, obj->line[i]);
    data->line.relay.size = 0;

    size = data->loop.size = obj->loopSize; unDevSize(size, data->loop);
    for(int i=0; i< size; ++i) unObjData(i, data->loop, obj->loop[i]);    

    size = data->output.size = obj->outputSize; unDevSize(size, data->output);
    for(int i=0; i< size; ++i) unObjData(i, data->output, obj->output[i]);
    //data->loop.vol.size = 0;

    size = data->env.size = obj->envSize;
    for(int i=0; i< size; ++i) unEnvData(i, data->env, obj->env[i]);
    data->env.hum.size = data->env.tem.size = size;

    data->rtuCount = obj->rtuCount;
    data->tg = obj->tg;
    data->lps = obj->lps;
    data->dc = obj->dc;
    data->hz = obj->hz;
    data->br = obj->br;
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