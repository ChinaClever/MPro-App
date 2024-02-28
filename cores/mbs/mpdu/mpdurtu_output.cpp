/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mpdurtu_output.h"

Mpdurtu_Output::Mpdurtu_Output(QObject *parent) : Mpdurtu_Loop{parent}
{

}

void Mpdurtu_Output::output_BaseUpdate(uint v[],int startReg)
{
    vshort vs; int size = OUTPUT_NUM - 6;
    appendData(size , v, vs);
    setRegs(startReg, vs);
}

void Mpdurtu_Output::output_BaseUpdate2(uint v[],int startReg)
{
    vshort vs; int size = OUTPUT_NUM - 6;
    appendData2(size , v, vs);
    setRegs(startReg, vs);
}

void Mpdurtu_Output::output_Updatesw()
{
    output_BaseUpdate(mDevData->output.relay.sw , MReg_OutputRelay);
}

void Mpdurtu_Output::output_Update()
{
    sAlarmUnit *unit = &(mDevData->output.cur);
    output_BaseUpdate(unit->value , MReg_OutputCurValue);
    output_BaseUpdate(unit->min , MReg_OutputCurMin);
    output_BaseUpdate(unit->max , MReg_OutputCurMax);
    output_BaseUpdate(unit->crMin , MReg_OutputCurCrMin);
    output_BaseUpdate(unit->crMax , MReg_OutputCurCrMax);
    output_BaseUpdate(mDevData->output.pf , MReg_OutputPowerFactor);
    output_BaseUpdate2(mDevData->output.ele , MReg_OutputEle);
}


void Mpdurtu_Output::output_ctrl(ushort addr, ushort address, ushort value)
{
    if(value > 2) return ;
    ushort reg = address - MReg_OutputRelay;
    if(reg < 49) {
        sDataItem unit;
        unit.addr = addr;
        unit.id = reg % 50 + 1;
        unit.type = DType::Output;
        unit.topic = DTopic::Relay;
        unit.subtopic = DSub::Value;

        if(value < 2) {
            uint *ptr = cm::devData(addr)->output.relay.sw;
            if(ptr[unit.id-1] == value) return;
        } setting(unit, value);
    }
//    else {
//        int id = (reg+1) % 50;
//        output_clearEle(addr, id);
//    }
}

void Mpdurtu_Output::output_setting(ushort addr, ushort address, ushort value)
{
    ushort reg = 1;
    sObjData *obj = &(cm::devData(addr)->output);
    sAlarmUnit *unit = nullptr; uint *ptr = nullptr;
    int id = 0; sDataItem it;
    it.type = DType::Output; it.addr = addr;

    unit = &(obj->cur); it.topic = DTopic::Cur;

    if( address >= MReg_OutputCurMax && address < MReg_OutputCurCrMin){
        id = address - MReg_OutputCurMax; reg = 1;
        ptr = unit->max; it.subtopic = DSub::VMax;
    }else if( address >= MReg_OutputCurCrMax && address < MReg_OutputPowerFactor){
        id = address - MReg_OutputCurCrMax; reg = 2;
        ptr = unit->crMax; it.subtopic = DSub::VCrMax;
    }else if( address >= MReg_OutputCurCrMin && address < MReg_OutputCurCrMax){
        id = address - MReg_OutputCurCrMin; reg = 3;
        ptr = unit->crMin; it.subtopic = DSub::VCrMin;
    }else if( address >= MReg_OutputCurMin && address < MReg_OutputCurMax){
        id = address - MReg_OutputCurMin; reg = 4;
        ptr = unit->min; it.subtopic = DSub::VMin;
    }

    it.id = id+1;

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret && ptr[id] != value) setting(it, value);
    if(ptr && ret) ptr[id] = value;
}
