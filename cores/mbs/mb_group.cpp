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
    setRegs(MbReg_GroupData, vs);
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
    setRegs(MbReg_GroupThreshol, vs);
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
    setRegs(MbReg_GroupRelay, vs);
}

void Mb_Group::group_ctrl(int id, ushort value)
{
    //QList<int> ids; sObjData *obj = &(mDevData->group);
    //ids = Data_Core::bulid()->outletByGroup(id);
    //sRelayUnit *it = &(obj->relay);
   // if(it->en[id]) OP_Core::bulid()->relaysCtrl(ids, value);

    sDataItem unit;
    unit.rw = 1;
    unit.id = id+1;
    unit.addr = 0;
    unit.value = value;
    unit.type = DType::Group;
    unit.topic = DTopic::Relay;
    unit.subtopic = DSub::Value;
    unit.txType = DTxType::TxModbus;
    if(value < 3) Set_Core::bulid()->setting(unit);
}

void Mb_Group::group_setting(ushort addr, ushort value)
{
    ushort reg = addr - MbReg_GroupThreshol;
    sObjData *obj = &(mDevData->group);
    sAlarmUnit *unit = nullptr;
    uint *ptr = nullptr;
    int id = reg%15;

    switch (reg/75) {
    case 0: unit = &(obj->pow); break;
    case 1: group_ctrl(id, value); return;
    default: cout << addr << reg << value; return;
    }

    reg = reg%15; switch (reg) {
    case 0: ptr = unit->en; break;
    case 1: ptr = unit->max; break;
    case 2: ptr = unit->crMax; break;
    case 3: ptr = unit->crMin; break;
    case 4: ptr = unit->min; break;
    default: cout << addr; break;
    }

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret) ptr[id] = value;
}
