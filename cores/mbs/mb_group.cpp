/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_group.h"
#include "data_core.h"

Mb_Group::Mb_Group(QObject *parent) : Mb_Output{parent}
{

}

void Mb_Group::group_dataUpdate()
{
    vshort vs; int size = GROUP_NUM+3;
    sObjData *obj = &(mDevData->group);
    appendData(size, obj->pow.value, vs);
    appendData(size, obj->artPow, vs);
    appendData(size, obj->pf, vs);
    appendData2(size-3, obj->ele, vs);
    appendData(size, obj->pow.alarm, vs);
    setRegs(mStartReg+MbReg_GroupData, vs);
}

void Mb_Group::group_thresholdUpdate()
{
    vshort vs; int size = GROUP_NUM+3;
    sAlarmUnit *obj = &(mDevData->group.pow);
    appendData(size, obj->en, vs);
    appendData(size, obj->max, vs);
    appendData(size, obj->crMax, vs);
    appendData(size, obj->crMin, vs);
    appendData(size, obj->min, vs);
    setRegs(mStartReg+MbReg_GroupThreshol, vs);
}

void Mb_Group::group_update()
{
    group_dataUpdate();
    group_relayUpdate();
    group_thresholdUpdate();
}

void Mb_Group::group_relayUpdate()
{
    sObjData *obj = &(mDevData->group);
    vshort vs; int size = obj->size;
    uint *ptr = obj->relay.reserve[3];
    for(int i=0; i<size; ++i) ptr[i] = 0xff;
    appendData(size, ptr, vs);
    setRegs(mStartReg+MbReg_GroupRelay, vs);
}

void Mb_Group::group_ctrl(ushort addr, int id, ushort value)
{
    sDataItem unit;
    unit.id = id+1;
    unit.addr = addr;
    unit.type = DType::Group;
    unit.topic = DTopic::Relay;
    unit.subtopic = DSub::Value;
    if(value < 3) setting(unit, value);
}

void Mb_Group::group_setting(ushort addr, ushort address, ushort value)
{
    ushort reg = address - MbReg_GroupThreshol;
    sObjData *obj = &(cm::devData(addr)->group);
    sAlarmUnit *unit = nullptr; uint *ptr = nullptr;
    int id = reg%15; sDataItem it; it.id = id+1;
    it.type = DType::Group; it.addr = addr;

    switch (reg/75) {
    case 0: unit = &(obj->pow); it.topic = DTopic::Pow; break;
    case 1: group_ctrl(addr, id, value); return;
    default: cout << addr << reg << value; return;
    }

    reg = reg/15; switch (reg) {
    case 0: ptr = unit->en; it.subtopic = DSub::EnAlarm; break;
    case 1: ptr = unit->max; it.subtopic = DSub::VMax; break;
    case 2: ptr = unit->crMax; it.subtopic = DSub::VCrMax; break;
    case 3: ptr = unit->crMin; it.subtopic = DSub::VCrMin; break;
    case 4: ptr = unit->min; it.subtopic = DSub::VMin; break;
    default: cout << addr << address; break;
    }

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret && ptr[id] != value) setting(it, value);
    if(ptr && ret) ptr[id] = value;
}
