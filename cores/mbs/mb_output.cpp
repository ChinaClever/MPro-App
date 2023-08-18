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
    for(int i=0; i<size; ++i) {
        output_dataObj(vs, i);
    } setRegs(MbReg_OutputValue, vs);
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
    setRegs(MbReg_OutputData, vs);
}

void Mb_Output::output_alarmUpdate()
{
    vshort vs; int size = OUTPUT_NUM+2;
    sObjData *obj = &(mDevData->output);
    appendData(size, obj->relay.alarm, vs);
    appendData(size, obj->cur.alarm, vs);
    appendData(size, obj->pow.alarm, vs);
    setRegs(MbReg_OutputAlarm, vs);
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
    setRegs(MbReg_OutputThreshol, vs);
}

void Mb_Output::output_relayUpdate()
{
    sObjData *obj = &(mDevData->output);
    vshort vs; int size = obj->size+1;
    uint *ptr = obj->relay.reserve[3];
    for(int i=0; i<size; ++i) ptr[i] = 0xff;
    appendData(size, ptr, vs);

    setRegs(MbReg_OutputRelay, vs);
    setRegs(MbReg_OutputEle-1, vs);
    setRegs(MbReg_DualCtr, vs);
}

void Mb_Output::output_update()
{
    output_objUpdate();
    output_dataUpdate();
    output_alarmUpdate();
    output_relayUpdate();
    output_thresholdUpdate();
}

void Mb_Output::output_dualCtrl(ushort addr, ushort value)
{
    ushort reg = addr - MbReg_DualCtr;
     int id = reg % 50 + 1;
    if(value < 3) {
        sDataItem unit;
        unit.rw = 1;
        unit.id = 2;
        unit.addr = 0;
        unit.type = id*2-1;
        unit.topic = DTopic::Relay;
        unit.subtopic = DSub::Relays;
        unit.txType = DTxType::TxModbus;
        unit.value = value;
        Set_Core::bulid()->setting(unit);
    }
}

void Mb_Output::output_ctrl(ushort addr, ushort value)
{
    if(value > 2) return ;
    ushort reg = addr - MbReg_OutputRelay;
    if(reg < 49) {
        //sRelayUnit *obj = &(mDevData->output.relay);
        //if(obj->en[id-1]) OP_Core::bulid()->relayCtrl(id, value);

        sDataItem unit;
        unit.rw = 1;
        unit.addr = 0;
        unit.value = value;
        unit.id = reg % 50 + 1;
        unit.type = DType::Output;
        unit.topic = DTopic::Relay;
        unit.subtopic = DSub::Value;
        unit.txType = DTxType::TxModbus;
        Set_Core::bulid()->setting(unit);
    } else {
        int id = (reg+1) % 50;
        OP_Core::bulid()->clearEle(id);
    }
}

void Mb_Output::output_setting(ushort addr, ushort value)
{
    ushort reg = addr - MbReg_OutputThreshol;
    sObjData *obj = &(mDevData->output);
    sAlarmUnit *unit = nullptr;
    uint *ptr = nullptr;
    int id = reg%50;

    switch (reg/250) {
    case 0: unit = &(obj->cur); break;
    case 1: unit = &(obj->pow); break;
    case 2: OP_Core::bulid()->setDelay(id+1, value); return;
    default: cout << addr << value; return;
    }

    reg = reg%250/50; switch (reg) {
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

