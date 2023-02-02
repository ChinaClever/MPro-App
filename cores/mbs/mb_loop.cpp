/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_loop.h"

Mb_Loop::Mb_Loop(QObject *parent) : Mb_Output{parent}
{

}


void Mb_Loop::loop_dataObj(vshort &vs, int id)
{
    sObjData *obj = &(mDevData->loop);
    vs << obj->vol.value[id];
    vs << obj->cur.value[id];
    vs << obj->pow.value[id];
    vs << obj->artPow[id];
    vs << obj->pf[id];
    vs << obj->ele[id]/0xffff;
    vs << obj->ele[id]%0xffff;
    vs << obj->relay.sw[id];
    vs << obj->reactivePow[id];
    vs << 0;
}

void Mb_Loop::loop_dataUpdate()
{
    vshort vs; loop_alarmUpdate();
    int size = mDevData->loop.size;
    for(int i=0; i<size; ++i) {
        loop_dataObj(vs, i);
    } setRegs(MbReg_LoopData, vs);
}

void Mb_Loop::loop_alarmObj(vshort &vs, int id)
{
    sObjData *obj = &(mDevData->loop);
    vs << obj->vol.alarm[id];
    vs << obj->cur.alarm[id];
    vs << obj->pow.alarm[id];
}

void Mb_Loop::loop_alarmUpdate()
{
    vshort vs; int size = mDevData->loop.size;
    for(int i=0; i<size; ++i) {
        loop_alarmObj(vs, i);
    } setRegs(MbReg_LoopAlarm, vs);
}

void Mb_Loop::loop_breakerUpdate()
{
    vshort vs;
    sObjData *obj = &(mDevData->loop);
    int size = obj->relay.size;
    for(int i=0; i<size; ++i) {
        vs << obj->relay.alarm[i];
    } setRegs(MbReg_LoopBreaker, vs);
}

void Mb_Loop::loop_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs)
{
    vs << unit.max[id];
    vs << unit.crMax[id];
    vs << unit.crMin[id];
    vs << unit.min[id];
    vs << unit.en[id];
}

void Mb_Loop::loop_thresholdUpdate()
{
    vshort vs; sObjData *obj = &(mDevData->loop);
    int size = mDevData->loop.size;
    for(int i=0; i<size; ++i) {
        loop_thresholdObj(obj->vol, i, vs);
        loop_thresholdObj(obj->cur, i, vs);
        loop_thresholdObj(obj->pow, i, vs);
    } setRegs(MbReg_LoopThreshol, vs);
}

void Mb_Loop::loop_update()
{
    loop_dataUpdate();
    loop_alarmUpdate();
    loop_breakerUpdate();
    loop_thresholdUpdate();
}




