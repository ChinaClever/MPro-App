#include "mb_loop.h"

Mb_Loop::Mb_Loop(QObject *parent) : Mb_Output{parent}
{

}

void Mb_Loop::mbLoopUpdate()
{
    upLoopData();
    mbOutputUpdate();
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
    appendData(obj->size, obj->cur.crMax, vs);
    setRegs(1202, vs);
}
