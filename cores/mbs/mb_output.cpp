/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_output.h"

Mb_Output::Mb_Output(QObject *parent) : Mb_Line{parent}
{

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
    appendData(size, unit.max, vs);
    appendData(size, unit.crMax, vs);
    appendData(size, unit.crMin, vs);
    appendData(size, unit.min, vs);
    appendData(size, unit.en, vs);
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

void Mb_Output::output_update()
{
    output_dataUpdate();
    output_alarmUpdate();
    output_thresholdUpdate();
}


