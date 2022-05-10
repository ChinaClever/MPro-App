/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_setting.h"

Mb_Setting::Mb_Setting(QObject *parent) : Mb_Env{parent}
{
    connect(this, &Modbus_SlaveObj::rcvDataSig, this, &Mb_Setting::rcvDataSlot);
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

void Mb_Setting::addrSet(vshort &values)
{
    if(1 == values.size()) {
        ushort addr = values.first();
        mDevData->info.modbusAddr = addr;
        setAddress(addr);

        /////////=================
    } else qDebug() << Q_FUNC_INFO << "modbus set addr err" << values;
}

void Mb_Setting::rcvDataSlot(int address, vshort values)
{
    switch (address) {
    case MbReg_SetAddr:

        break;
    default:
        break;
    }
}
