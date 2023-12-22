/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_output.h"

Mb_Output::Mb_Output(QObject *parent) : Mb_Loop{parent}
{

}

void Mb_Output::output_dataObj(vshort &vs, int id)
{
    sObjData *obj = &(mDevData->output);
    vs << obj->cur.value[id];
    vs << obj->pow.value[id];
    vs << obj->artPow[id];
    vs << obj->pf[id];
    vs << obj->relay.sw[id];
}

void Mb_Output::output_objUpdate()
{
    vshort vs; int size = mDevData->output.size;
    for(int i=0; i<size; ++i) output_dataObj(vs, i);
    setRegs(mStartReg + MbReg_OutputValue, vs);
}

void Mb_Output::output_dataUpdate()
{
    vshort vs; int size = OUTPUT_NUM+2;
    sObjData *obj = &(mDevData->output);
    appendData(size, obj->cur.value, vs);
    appendData(size, obj->pow.value, vs);
    appendData(size, obj->artPow, vs);
    appendData(size, obj->pf, vs);
    appendData2(size, obj->ele, vs);       
    appendData(size, obj->relay.sw, vs);
    appendData(size, obj->reactivePow, vs);
    setRegs(mStartReg + MbReg_OutputData, vs);
}

void Mb_Output::output_alarmUpdate()
{
    vshort vs; int size = OUTPUT_NUM+2;
    sObjData *obj = &(mDevData->output);
    appendData(size, obj->relay.alarm, vs);
    appendData(size, obj->cur.alarm, vs);
    appendData(size, obj->pow.alarm, vs);
    setRegs(mStartReg + MbReg_OutputAlarm, vs);
}

void Mb_Output::output_thresholdObj(const sAlarmUnit &unit, vshort &vs)
{
    int size = OUTPUT_NUM+2;
    appendData(size, unit.en, vs);
    appendData(size, unit.max, vs);
    appendData(size, unit.crMax, vs);
    appendData(size, unit.crMin, vs);
    appendData(size, unit.min, vs);
}

void Mb_Output::output_thresholdUpdate()
{
    vshort vs; int size = OUTPUT_NUM+2;
    sObjData *obj = &(mDevData->output);
    output_thresholdObj(obj->cur, vs);
    output_thresholdObj(obj->pow, vs);
    appendData(size, obj->relay.powerUpDelay, vs);
    setRegs(mStartReg + MbReg_OutputThreshol, vs);
}

void Mb_Output::output_relayUpdate()
{
    sObjData *obj = &(mDevData->output);
    vshort vs; int size = obj->size+1;
    uint *ptr = obj->relay.reserve[3];
    for(int i=0; i<size; ++i) ptr[i] = 0xff;
    appendData(size, ptr, vs);

    setRegs(mStartReg + MbReg_OutputRelay, vs);
    setRegs(mStartReg + MbReg_OutputEle-1, vs);
    setRegs(mStartReg + MbReg_DualCtr, vs);
}

void Mb_Output::output_update()
{
    output_objUpdate();
    output_dataUpdate();
    output_alarmUpdate();
    output_relayUpdate();
    output_thresholdUpdate();
}

void Mb_Output::output_dualCtrl(ushort addr, ushort address, ushort value)
{
    ushort reg = address - MbReg_DualCtr;
     int id = reg % 50 + 1;
    if(value < 3) {
        sDataItem unit;
        unit.id = 2;
        unit.addr = addr;
        unit.type = id*2-1;
        unit.topic = DTopic::Relay;
        unit.subtopic = DSub::Relays;
        setting(unit, value);
    }
}


void Mb_Output::output_clearEle(uchar addr, uchar id)
{
    sDataItem unit;
    unit.id = id;
    unit.addr = addr;
    unit.type = DType::Output;
    unit.topic = DTopic::Ele;
    unit.subtopic = DSub::Value;
    if(1 == mDevData->cfg.param.devSpec) unit.type = DType::Loop;
    setting(unit, 0); //OP_Core::bulid()->clearEle(0);
}


void Mb_Output::output_ctrl(ushort addr, ushort address, ushort value)
{
    if(value > 2) return ;
    ushort reg = address - MbReg_OutputRelay;
    if(reg < 49) {
        sDataItem unit;
        unit.addr = addr;
        unit.id = reg % 50 + 1;
        unit.type = DType::Output;
        unit.topic = DTopic::Relay;
        unit.subtopic = DSub::Value;

        if(value < 2) {
            uint *ptr = cm::devData(addr)->output.relay.sw;
            if(ptr[unit.id] == value) return;
        } setting(unit, value);
    } else {
        int id = (reg+1) % 50;
        output_clearEle(addr, id);
    }
}


void Mb_Output::output_upDelay(uchar addr, uchar id, ushort value)
{
    sDataItem unit;
    unit.id = id;
    unit.addr = addr;
    unit.type = DType::Output;
    unit.topic = DTopic::Relay;
    unit.subtopic = DSub::UpDelay;

    uint *ptr = cm::devData(addr)->output.relay.powerUpDelay;
    if(ptr[unit.id] != value) {setting(unit, value);} ptr[id] = value;
}

void Mb_Output::output_setting(ushort addr, ushort address, ushort value)
{
    ushort reg = address - MbReg_OutputThreshol;
    sObjData *obj = &(cm::devData(addr)->output);
    sAlarmUnit *unit = nullptr; uint *ptr = nullptr;
    int id = reg%50+1; sDataItem it; it.id = id;
    it.type = DType::Output; it.addr = addr;

    switch (reg/250) {
    case 0: unit = &(obj->cur); it.topic = DTopic::Cur; break;
    case 1: unit = &(obj->pow); it.topic = DTopic::Pow; break;
    case 2: output_upDelay(addr, id, value); return;
    default: cout << addr << address << value; return;
    }

    reg = reg%250/50; switch (reg) {
    case 0: ptr = unit->en; it.subtopic = DSub::EnAlarm; break;
    case 1: ptr = unit->max; it.subtopic = DSub::VMax; break;
    case 2: ptr = unit->crMax; it.subtopic = DSub::VCrMax; break;
    case 3: ptr = unit->crMin; it.subtopic = DSub::VCrMin; break;
    case 4: ptr = unit->min; it.subtopic = DSub::VMin; break;
    default: cout << addr << address; return;
    }

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret && ptr[id] != value) setting(it, value);
    if(ptr) ptr[id] = value;
}

