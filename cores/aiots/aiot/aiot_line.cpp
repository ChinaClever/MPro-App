/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "aiot_line.h"

Aiot_Line::Aiot_Line()
{

}


void Aiot_Line::linePost()
{
    lineVols();
    lineActivePowers();
}

void Aiot_Line::lineVols()
{
    double unit = COM_RATE_VOL;
    QString key = "lineVol";
    QString module = "LineParameters";
    int size = mDevData->line.vol.size;
    uint *ptr = mDevData->line.vol.value;
    appendArrayProperty(ptr, size, unit, key, module);
}

void Aiot_Line::lineActivePowers()
{
    double unit = COM_RATE_POW;
    QString key = "linePow";
    QString module = "LineParameters";
    int size = mDevData->line.pow.size;
    uint *ptr = mDevData->line.pow.value;
    appendArrayProperty(ptr, size, unit, key, module);
}
