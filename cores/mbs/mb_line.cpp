/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_line.h"

Mb_Line::Mb_Line(QObject *parent) : Mb_Object{parent}
{

}


void Mb_Line::line_update()
{
    line_dataUpdate();
    line_alarmUpdate();
    line_thresholdUpdate();
}

void Mb_Line::line_dataObj(vshort &vs, int id)
{
    sObjData *obj = &(mDevData->line);
    vs << obj->vol.value[id];
    vs << obj->cur.value[id];
    vs << obj->pow.value[id];
    vs << obj->artPow[id];
    vs << obj->pf[id];
    vs << (obj->ele[id] >> 16);
    vs << (obj->ele[id] & 0xffff);
    vs << obj->reactivePow[id];
    vs << obj->lineVol[id];
    vs << 0;
}

void Mb_Line::line_dataUpdate()
{
    vshort vs; int size = mDevData->line.size;
    for(int i=0; i<size; ++i) line_dataObj(vs, i);
    setRegs(mStartReg + MbReg_LineData, vs);
}

void Mb_Line::line_alarmObj(vshort &vs, int id)
{
    sObjData *obj = &(mDevData->line);
    vs << obj->vol.alarm[id];
    vs << obj->cur.alarm[id];
    vs << obj->pow.alarm[id];
}

void Mb_Line::line_alarmUpdate()
{
    vshort vs; int size = mDevData->line.size;
    for(int i=0; i<size; ++i) line_alarmObj(vs, i);
    setRegs(mStartReg + MbReg_LineAlarm, vs);
}

void Mb_Line::line_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs)
{
    vs << unit.en[id];
    vs << unit.max[id];
    vs << unit.crMax[id];
    vs << unit.crMin[id];
    vs << unit.min[id];
}

void Mb_Line::line_thresholdUpdate()
{
    vshort vs; sObjData *obj = &(mDevData->line);
    int size = mDevData->line.size;
    for(int i=0; i<size; ++i) {
        line_thresholdObj(obj->vol, i, vs);
        line_thresholdObj(obj->cur, i, vs);
        line_thresholdObj(obj->pow, i, vs);
    } setRegs(mStartReg + MbReg_LineThreshol, vs);
}

void Mb_Line::line_setting(ushort addr, ushort address, ushort value)
{
    ushort reg = address - MbReg_LineThreshol;
    sObjData *obj = &(cm::devData(addr)->line);
    sAlarmUnit *unit = nullptr; uint *ptr = nullptr;
    int id = reg/15+1; sDataItem it; it.id = id;
    it.type = DType::Line; it.addr = addr;

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
    default: cout << addr << address; break;
    }

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret && (ptr[id] != value)) setting(it, value);
    if(ptr) ptr[id] = value;
}





