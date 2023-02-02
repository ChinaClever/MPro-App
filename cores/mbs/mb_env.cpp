/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_env.h"

Mb_Env::Mb_Env(QObject *parent) : Mb_Loop{parent}
{

}


void Mb_Env::env_dataUpdate()
{
    vshort vs; int size = SENOR_NUM;
    sEnvData *obj = &(mDevData->env);
    appendData(size, obj->tem.value, vs);
    appendData(size, obj->hum.value, vs);
    setRegs(MbReg_EnvData, vs);
}

void Mb_Env::env_alarmUpdate()
{
    vshort vs; int size = SENOR_NUM;
    sEnvData *obj = &(mDevData->env);
    appendData(size, obj->tem.alarm, vs);
    appendData(size, obj->hum.alarm, vs);
    setRegs(MbReg_EnvAlarm, vs);
}


void Mb_Env::env_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs)
{
    vs << unit.max[id];
    vs << unit.crMax[id];
    vs << unit.crMin[id];
    vs << unit.min[id];
    vs << unit.en[id];
}


void Mb_Env::env_thresholdUpdate()
{
    vshort vs; int size = SENOR_NUM; //obj.size;
    sEnvData *obj = &(mDevData->env);
    for(int i=0; i<size; ++i) {
        env_thresholdObj(obj->tem, i, vs);
        env_thresholdObj(obj->hum, i, vs);
    } setRegs(MbReg_EnvThreshol, vs);
}

void Mb_Env::env_sensorUpdate()
{
    vshort vs;
    sEnvData *obj = &(mDevData->env);
    vs << obj->door[0];
    vs << obj->door[1];
    vs << obj->water[0];
    vs << obj->smoke[0];
    setRegs(MbReg_EnvSensor, vs);
}

void Mb_Env::env_update()
{
    env_dataUpdate();
    env_alarmUpdate();
    env_sensorUpdate();
    env_thresholdUpdate();
}





