/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_control.h"

Mb_Control::Mb_Control(QObject *parent)
    : Mb_Alarm{parent}
{

}

void Mb_Control::mbCtrlUpdate()
{
    upRelayState();
}


void Mb_Control::upRelayState()
{
    sObjData *obj = &(mDevData->output);
    vshort vs; int size = obj->size;
    appendData(size, obj->relay.sw, vs);
    setRegs(MbReg_Controls, vs);
}
