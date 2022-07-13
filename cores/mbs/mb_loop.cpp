/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_loop.h"

Mb_Loop::Mb_Loop(QObject *parent) : Mb_Output{parent}
{

}

void Mb_Loop::mbLoopUpdate()
{
    upLoopData();
    upLoopThreshold();
}

void Mb_Loop::upLoopData()
{
    sObjData *obj = &(mDevData->loop);
    vshort vs; int size = obj->size;
    vs << 200 << size;

    appendData(size, obj->cur.value, vs);
    setRegs(MbReg_Loops, vs);
}

void Mb_Loop::upLoopThreshold()
{
    vshort vs;
    sObjData *obj = &(mDevData->loop);
    appendData(obj->size, obj->cur.max, vs);
    setRegs(MbReg_SetLoop, vs); vs.clear();

    appendData(obj->size, obj->cur.crMax, vs);
    setRegs(1214, vs); vs.clear();

    appendData(obj->size, obj->cur.min, vs);
    setRegs(1226, vs); vs.clear();
}
