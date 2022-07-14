/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_output.h"

Mb_Output::Mb_Output(QObject *parent) : Mb_Line{parent}
{

}

void Mb_Output::mbOutputUpdate()
{
    upOutputData();
    upOutputThreshold();
}

void Mb_Output::upOutputData()
{
    vshort vs; int size = OUTPUT_NUM;
    sObjData *obj = &(mDevData->output);
    appendData(size, obj->cur.value, vs);
    appendData2(size, obj->ele, vs);

    appendData(size, obj->pow.value, vs);
    appendData(size, obj->reactivePow, vs);
    appendData(size, obj->cur.rated, vs);
    setRegs(MbReg_Outputs, vs);
}

void Mb_Output::upOutputThreshold()
{
    vshort vs;
    sObjData *obj = &(mDevData->output);
    appendAlarm(obj->cur, vs);
    setRegs(MbReg_SetOutput, vs); vs.clear();

    appendData(obj->size, obj->cur.crMax, vs);
    setRegs(1153, vs);
}
