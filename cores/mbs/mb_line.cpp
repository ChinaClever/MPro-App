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
    vs << obj->ele[id]/0xffff;
    vs << obj->ele[id]%0xffff;
    vs << obj->reactivePow[id];
    vs << obj->lineVol[id];
    vs << 0;
}

void Mb_Line::line_dataUpdate()
{
    vshort vs; int size = mDevData->line.size;
    for(int i=0; i<size; ++i) {
        line_dataObj(vs, i);
    } setRegs(MbReg_LineData, vs);
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
    for(int i=0; i<size; ++i) {
        line_alarmObj(vs, i);
    } setRegs(MbReg_LineAlarm, vs);
}

void Mb_Line::line_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs)
{
    vs << unit.max[id];
    vs << unit.crMax[id];
    vs << unit.crMin[id];
    vs << unit.min[id];
    vs << unit.en[id];
}

void Mb_Line::line_thresholdUpdate()
{
    vshort vs; sObjData *obj = &(mDevData->line);
    int size = mDevData->line.size;
    for(int i=0; i<size; ++i) {
        line_thresholdObj(obj->vol, i, vs);
        line_thresholdObj(obj->cur, i, vs);
        line_thresholdObj(obj->pow, i, vs);
    } setRegs(MbReg_LineThreshol, vs);
}









