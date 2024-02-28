/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mpdurtu_setting.h"
#include "app_core.h"
#include "../mb_core.h"
#include "cfg_core.h"


Mpdurtu_Setting::Mpdurtu_Setting(QObject *parent) : Mpdurtu_Env{parent}
{
    connect(this, &Modbus_SlaveObj::registerDataSig, this, &Mpdurtu_Setting::registerRecvSlot);
}

void Mpdurtu_Setting::mbSetUpdate()
{
//    upSetData();
}


void Mpdurtu_Setting::registerRecvSlot(int address, ushort value)
{
    ushort reg = address % MReg_MaxSize;
    ushort addr = address / MReg_MaxSize;
    if(reg < MReg_OutputRelay) return;
    else if(reg < MReg_LineCurValue) output_ctrl(addr, reg, value);
    else if(reg >=MReg_LineCurMin && reg < MReg_LineVolValue) line_setting(addr, reg, value);
    else if(reg >=MReg_LineVolMin && reg < MReg_LinePower) line_setting(addr, reg, value);
    else if(reg >=MReg_LoopCurMin && reg < MReg_LoopRelay) loop_setting(addr, reg, value);
    else if(reg >=MReg_OutputCurMin && reg < MReg_OutputPowerFactor) output_setting(addr, reg, value);
    else if(reg >=MReg_TemMin && reg < MReg_Hum) env_setting(addr, reg, value);
    else if(reg >=MReg_HumMin && reg < MReg_Door) env_setting(addr, reg, value);

}
