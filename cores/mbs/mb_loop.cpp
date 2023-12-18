/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_loop.h"

Mb_Loop::Mb_Loop(QObject *parent) : Mb_Line{parent}
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
    vs << (obj->ele[id] >> 16);
    vs << (obj->ele[id] & 0xffff);
    vs << obj->relay.sw[id];
    vs << obj->reactivePow[id];
    vs << 0;
}

void Mb_Loop::loop_dataUpdate()
{
    vshort vs; loop_alarmUpdate();
    int size = mDevData->loop.size;
    for(int i=0; i<size; ++i) loop_dataObj(vs, i);
    setRegs(mStartReg + MbReg_LoopData, vs);
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
    for(int i=0; i<size; ++i) loop_alarmObj(vs, i);
    setRegs(mStartReg + MbReg_LoopAlarm, vs);
}

void Mb_Loop::loop_breakerUpdate()
{
    vshort vs; sObjData *obj = &(mDevData->loop); int size = obj->relay.size;
    for(int i=0; i<size; ++i) vs << obj->relay.alarm[i];
    setRegs(mStartReg + MbReg_LoopBreaker, vs);
}

void Mb_Loop::loop_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs)
{
    vs << unit.en[id];
    vs << unit.max[id];
    vs << unit.crMax[id];
    vs << unit.crMin[id];
    vs << unit.min[id];
}

void Mb_Loop::loop_thresholdUpdate()
{
    vshort vs; sObjData *obj = &(mDevData->loop);
    int size = mDevData->loop.size;
    for(int i=0; i<size; ++i) {
        loop_thresholdObj(obj->vol, i, vs);
        loop_thresholdObj(obj->cur, i, vs);
        loop_thresholdObj(obj->pow, i, vs);
    } setRegs(mStartReg + MbReg_LoopThreshol, vs);
}

void Mb_Loop::loop_update()
{
    loop_dataUpdate();
    loop_alarmUpdate();
    loop_breakerUpdate();
    loop_thresholdUpdate();
}

void Mb_Loop::loop_setting(ushort addr, ushort address, ushort value)
{
    ushort reg = address - MbReg_LoopThreshol;
    sObjData *obj = &(cm::devData(addr)->loop);
    sAlarmUnit *unit = nullptr; uint *ptr = nullptr;
    int id = reg/15; sDataItem it; it.id = id+1;
    it.type = DType::Loop; it.addr = addr;

    switch (reg%15/5) {
    case 0: unit = &(obj->vol); it.topic = DTopic::Vol; break;
    case 1: unit = &(obj->cur); it.topic = DTopic::Cur; break;
    case 3: unit = &(obj->pow); it.topic = DTopic::Pow; break;
    default: cout << addr << address; return;
    }

    reg = reg % 5; switch (reg) {
    case 0: ptr = unit->en; it.subtopic = DSub::EnAlarm; break;
    case 1: ptr = unit->max; it.subtopic = DSub::VMax; break;
    case 2: ptr = unit->crMax; it.subtopic = DSub::VCrMax; break;
    case 3: ptr = unit->crMin; it.subtopic = DSub::VCrMin; break;
    case 4: ptr = unit->min; it.subtopic = DSub::VMin; break;
    default: cout << addr << address;; break;
    }

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret && (ptr[id] != value)) setting(it, value);
    if(ptr) ptr[id] = value;
}


