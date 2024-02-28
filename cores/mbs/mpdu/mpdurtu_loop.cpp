/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mpdurtu_loop.h"

Mpdurtu_Loop::Mpdurtu_Loop(QObject *parent) : Mpdurtu_Line{parent}
{

}

void Mpdurtu_Loop::loop_BaseUpdate(uint v[],int startReg)
{
    vshort vs; int size = mDevData->loop.size;
    appendData(size , v, vs);
    setRegs(startReg, vs);
}

void Mpdurtu_Loop::loop_BaseUpdate2(uint v[],int startReg)
{
    vshort vs; int size = mDevData->loop.size;
    appendData2(size , v, vs);
    setRegs(startReg, vs);
}

void Mpdurtu_Loop::loop_Update()
{
    sAlarmUnit *unit = &(mDevData->loop.cur);
    loop_BaseUpdate(unit->value , MReg_LoopCurValue);
    loop_BaseUpdate(unit->min , MReg_LoopCurMin);
    loop_BaseUpdate(unit->max , MReg_LoopCurMax);
    loop_BaseUpdate(unit->crMin , MReg_LoopCurCrMin);
    loop_BaseUpdate(unit->crMax , MReg_LoopCurCrMax);
    loop_BaseUpdate(mDevData->loop.relay.sw , MReg_LoopRelay);
    loop_BaseUpdate2(mDevData->loop.ele , MReg_LoopEle);
}

void Mpdurtu_Loop::loop_setting(ushort addr, ushort address, ushort value)
{
    ushort reg = 1;
    sObjData *obj = &(cm::devData(addr)->loop);
    sAlarmUnit *unit = nullptr; uint *ptr = nullptr;
    int id = 0; sDataItem it;
    it.type = DType::Loop; it.addr = addr;

    unit = &(obj->cur); it.topic = DTopic::Cur;

    if(address >= MReg_LoopCurMax && address < MReg_LoopCurCrMin){
        id = address - MReg_LoopCurMax;
        ptr = unit->max; it.subtopic = DSub::VMax;
        reg = 1;
    }else if(address >= MReg_LoopCurCrMax && address < MReg_LoopRelay){
        id = address - MReg_LoopCurCrMax;
        ptr = unit->crMax; it.subtopic = DSub::VCrMax;
        reg = 2;
    }else if(address >= MReg_LoopCurCrMin && address < MReg_LoopCurCrMax){
        id = address - MReg_LoopCurCrMin;
        ptr = unit->crMin; it.subtopic = DSub::VCrMin;
        reg = 3;
    }else if(address >= MReg_LoopCurMin && address < MReg_LoopCurMax){
        id = address - MReg_LoopCurMin;
        ptr = unit->min; it.subtopic = DSub::VMin;
        reg = 4;
    }
    it.id = id+1;

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret && (ptr[id] != value)) setting(it, value);
    if(ptr && ret) ptr[id] = value;
}
