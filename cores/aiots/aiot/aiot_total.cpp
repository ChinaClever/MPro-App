/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "aiot_total.h"

Aiot_Total::Aiot_Total()
{

}

void Aiot_Total::totalPost()
{
    double unit = COM_RATE_POW;
    QString key = "totalActivePower";
    QString module = "InputTotal";
    double v = mDevData->tg.pow.value / unit;
    appendProperty(key, v, module);

    key = "totalApparentPower";
    v = mDevData->tg.artPow / unit;
    appendProperty(key, v, module);

    key = "totalPowerFactor";
    v = mDevData->tg.pf ; // / COM_RATE_PF;
    appendProperty(key, v, module);

    key = "totalActiveEnergy";
    v = mDevData->tg.ele / COM_RATE_ELE;
    appendProperty(key, v, module);
}
