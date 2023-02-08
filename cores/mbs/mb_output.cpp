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
    vshort vs; int size = obj->size;
    appendData(size, obj->relay.sw, vs);
    setRegs(MbReg_OutputRelay, vs);
}

void Mb_Output::output_update()
{
    output_objUpdate();
    output_dataUpdate();
    output_alarmUpdate();
    output_relayUpdate();
    output_thresholdUpdate();
}

void Mb_Output::output_ctrl(ushort addr, ushort value)
{
    ushort reg = addr - MbReg_OutputRelay;
    int id = reg % 50 + 1;
    if(reg > 50) {
        OP_Core::bulid()->clearEle(id);
    } else {
         //sRelayUnit *obj = &(mDevData->output.relay);
        //if(obj->en[id-1]) OP_Core::bulid()->relayCtrl(id, value);

        sDataItem unit;
        unit.rw = 1;
        unit.id = id;
        unit.addr = 0;
        unit.type = DType::Output;
        unit.topic = DTopic::Relay;
        unit.subtopic = DSub::Value;
        unit.txType = DTxType::TxSnmp;
        unit.value = value;
        Set_Core::bulid()->setting(unit);
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
    default: OP_Core::bulid()->setDelay(id+1, value); return;
    }

    switch (reg%250/50) {
    case 0: ptr = unit->en; break;
    case 1: ptr = unit->max; break;
    case 2: ptr = unit->crMax; break;
    case 3: ptr = unit->crMin; break;
    case 4: ptr = unit->min; break;
    default: cout << addr; break;
    } if(ptr) ptr[id] = value;
}

