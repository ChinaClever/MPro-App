/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_setting.h"

Mb_Setting::Mb_Setting(QObject *parent) : Mb_Env{parent}
{

}

void Mb_Setting::mbSetUpdate()
{
    upSetData();
}

void Mb_Setting::upSetData()
{
    vshort vs;
    vs << mDevData->info.modbusAddr;
    vs << mDevData->info.buzzerSw;
    vs << mDevData->info.drySw;
    setRegs(MbReg_Settings, vs);
}
