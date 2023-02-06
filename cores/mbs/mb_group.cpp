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
    vshort vs; int size = GROUP_NUM+2;
    sObjData *obj = &(mDevData->group);
    appendData(size, obj->pow.value, vs);
    appendData(size, obj->artPow, vs);
    appendData(size, obj->pf, vs);
    appendData2(size, obj->ele, vs);
    appendData(size, obj->pow.alarm, vs);
    setRegs(MbReg_GroupData, vs);
}

void Mb_Group::group_thresholdUpdate()
{
    vshort vs; int size = GROUP_NUM+2;
    sAlarmUnit *obj = &(mDevData->group.pow);
    appendData(size, obj->en, vs);
    appendData(size, obj->max, vs);
    appendData(size, obj->crMax, vs);
    appendData(size, obj->crMin, vs);
    appendData2(size, obj->min, vs);
    setRegs(MbReg_GroupThreshol, vs);
}

void Mb_Group::group_update()
{
    group_dataUpdate();
    group_thresholdUpdate();
}

void Mb_Group::group_ctrl(int id, ushort value)
{
    QList<int> ids; sObjData *obj = &(mDevData->group);
    ids = Data_Core::bulid()->outletByGroup(id);
    sRelayUnit *it = &(obj->relay);
    if(it->en[id]) OP_Core::bulid()->relaysCtrl(ids, value);
}

void Mb_Group::group_setting(ushort addr, ushort value)
{
    ushort reg = addr - MbReg_GroupThreshol;
    sObjData *obj = &(mDevData->group);
    sAlarmUnit *unit = nullptr;
    uint *ptr = nullptr;
    int id = reg%10;

    switch (reg/50) {
    case 0: unit = &(obj->pow); break;
    default: group_ctrl(id, value); return;
    }

    switch (reg%50/10) {
    case 0: ptr = unit->en; break;
    case 1: ptr = unit->max; break;
    case 2: ptr = unit->crMax; break;
    case 3: ptr = unit->crMin; break;
    case 4: ptr = unit->min; break;
    default: cout << addr; break;
    } if(ptr) ptr[id] = value;
}
