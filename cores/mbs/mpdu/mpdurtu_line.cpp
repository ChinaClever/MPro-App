/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mpdurtu_line.h"

Mpdurtu_Line::Mpdurtu_Line(QObject *parent) : Mpdurtu_Object{parent}
{

}

void Mpdurtu_Line::line_BaseUpdate(uint v[],int startReg)
{
    vshort vs; int size = mDevData->line.size;
    appendData(size , v, vs);
    setRegs(startReg, vs);
}

void Mpdurtu_Line::line_BaseUpdate2(uint v[],int startReg)
{
    vshort vs; int size = mDevData->line.size;
    appendData2(size , v, vs);
    setRegs(startReg, vs);
}

void Mpdurtu_Line::line_Update()
{
    sAlarmUnit *unit = &(mDevData->line.cur);
    line_BaseUpdate(unit->value , MReg_LineCurValue);
    line_BaseUpdate(unit->min , MReg_LineCurMin);
    line_BaseUpdate(unit->max , MReg_LineCurMax);
    line_BaseUpdate(unit->crMin , MReg_LineCurCrMin);
    line_BaseUpdate(unit->crMax , MReg_LineCurCrMax);
    unit = &(mDevData->line.vol);
    line_BaseUpdate(unit->value , MReg_LineVolValue);
    line_BaseUpdate(unit->min , MReg_LineVolMin);
    line_BaseUpdate(unit->max , MReg_LineVolMax);
    line_BaseUpdate(unit->crMin , MReg_LineVolCrMin);
    line_BaseUpdate(unit->crMax , MReg_LineVolCrMax);
    unit = &(mDevData->line.pow);
    line_BaseUpdate(unit->value , MReg_LinePower);
    line_BaseUpdate(mDevData->line.pf , MReg_LinePowerFactor);
    line_BaseUpdate2(mDevData->line.ele , MReg_LineEle);
}

void Mpdurtu_Line::line_setting(ushort addr, ushort address, ushort value)
{
    ushort reg = 1;
    sObjData *obj = &(cm::devData(addr)->line);
    sAlarmUnit *unit = nullptr; uint *ptr = nullptr;
    sDataItem it;int id = 0;
    it.type = DType::Line; it.addr = addr;

    if(address >= MReg_LineVolMin && address < MReg_LinePower){
        unit = &(obj->vol); it.topic = DTopic::Vol;
    }else if(address >= MReg_LineCurMin && address < MReg_LineVolValue){
        unit = &(obj->cur); it.topic = DTopic::Cur;
    }

    if((address >= MReg_LineCurMax && address < MReg_LineCurCrMin) || (address >= MReg_LineVolMax && address < MReg_LineVolCrMin)){
        if(it.topic == DTopic::Vol){id = address - MReg_LineVolMax;}
        else{id = address - MReg_LineCurMax;}
        ptr = unit->max; it.subtopic = DSub::VMax;
        reg = 1;
    }else if((address >= MReg_LineCurCrMax && address < MReg_LineVolValue) || (address >= MReg_LineVolCrMax && address < MReg_LinePower)){
        if(it.topic == DTopic::Vol){id = address - MReg_LineVolCrMax;}
        else{id = address - MReg_LineCurCrMax;}
        ptr = unit->crMax; it.subtopic = DSub::VCrMax;
        reg = 2;
    }else if((address >= MReg_LineCurCrMin && address < MReg_LineCurCrMax) || (address >= MReg_LineVolCrMin && address < MReg_LineVolCrMax)){
        if(it.topic == DTopic::Vol){id = address - MReg_LineVolCrMin;}
        else{id = address - MReg_LineCurCrMin;}
        ptr = unit->crMin; it.subtopic = DSub::VCrMin;
        reg = 3;
    }else if((address >= MReg_LineCurMin && address < MReg_LineCurMax) || (address >= MReg_LineVolMin && address < MReg_LineVolMax)){
        if(it.topic == DTopic::Vol){id = address - MReg_LineVolMin;}
        else{id = address - MReg_LineCurMin;}
        ptr = unit->min; it.subtopic = DSub::VMin;
        reg = 4;
    }
    it.id = id+1;

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret && (ptr[id] != value)) setting(it, value);
    if(ptr && ret) ptr[id] = value;
}


