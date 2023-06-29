/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "aiot_loop.h"

Aiot_Loop::Aiot_Loop()
{

}

void Aiot_Loop::loopPost()
{
   loopBreakers();
    loopActivePowers();
}

void Aiot_Loop::loopBreakers()
{
    double unit = 1;
    QString key = "breaker";
    QString module = "LoopParameters";
    int size = mDevData->loop.relay.size;
    uint *ptr = mDevData->loop.relay.sw;
    appendArrayProperty(ptr, size, unit, key, module);
}

void Aiot_Loop::loopActivePowers()
{
    double unit = COM_RATE_POW;
    QString key = "loopActivePower";
    QString module = "LoopParameters";
    int size = mDevData->loop.pow.size;
    uint *ptr = mDevData->loop.pow.value;
    appendArrayProperty(ptr, size, unit, key, module);
}
