/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_env.h"

Mb_Env::Mb_Env(QObject *parent) : Mb_Group{parent}
{

}


void Mb_Env::env_dataUpdate()
{
    vshort vs; int size = SENOR_NUM;
    sEnvData *obj = &(mDevData->env);
    for(int i=0; i<size; ++i) {
        if(obj->isInsert[i]) {
            vs << obj->tem.value[i];
            vs << obj->hum.value[i];
        } else vs << 0 << 0;
    }setRegs(mStartReg+MbReg_EnvData, vs);
}

void Mb_Env::env_alarmUpdate()
{
    vshort vs; int size = SENOR_NUM;
    sEnvData *obj = &(mDevData->env);
    for(int i=0; i<size; ++i) {
        vs << obj->tem.alarm[i];
        vs << obj->hum.alarm[i];
    }setRegs(mStartReg+MbReg_EnvAlarm, vs);
}


void Mb_Env::env_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs)
{
    vs << unit.en[id];
    vs << unit.max[id];
    vs << unit.crMax[id];
    vs << unit.crMin[id];
    vs << unit.min[id];
}


void Mb_Env::env_thresholdUpdate()
{
    vshort vs; int size = SENOR_NUM;
    sEnvData *obj = &(mDevData->env);
    for(int i=0; i<size; ++i) {
        env_thresholdObj(obj->tem, i, vs);
        env_thresholdObj(obj->hum, i, vs);
    } setRegs(mStartReg+MbReg_EnvThreshol, vs);
}

void Mb_Env::env_sensorUpdate()
{
    vshort vs;
    sEnvData *obj = &(mDevData->env);
    vs << obj->door[0];
    vs << obj->door[1];
    vs << obj->water[0];
    vs << obj->smoke[0];
    setRegs(mStartReg+MbReg_EnvSensor, vs);
}

void Mb_Env::env_update()
{
    env_dataUpdate();
    env_alarmUpdate();
    env_sensorUpdate();
    env_thresholdUpdate();
}

void Mb_Env::env_setting(ushort addr, ushort address, ushort value)
{
    ushort reg = address - MbReg_EnvThreshol;
    sEnvData *obj = &(cm::devData(addr)->env);
    sAlarmUnit *unit = nullptr; uint *ptr = nullptr;
    int id = reg/10; sDataItem it; it.id = id+1;
    it.type = DType::Env; it.addr = addr;

    switch (reg%10/5) {
    case 0: unit = &(obj->tem); it.topic = DTopic::Tem; break;
    case 1: unit = &(obj->hum); it.topic = DTopic::Hum; break;
    default: cout << addr << address; return;
    }

    reg = reg % 5; switch (reg) {
    case 0: ptr = unit->en; it.subtopic = DSub::EnAlarm; break;
    case 1: ptr = unit->max; it.subtopic = DSub::VMax; break;
    case 2: ptr = unit->crMax; it.subtopic = DSub::VCrMax; break;
    case 3: ptr = unit->crMin; it.subtopic = DSub::VCrMin; break;
    case 4: ptr = unit->min; it.subtopic = DSub::VMin; break;
    default: cout << addr << address; break;
    }

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret && ptr[id] != value) setting(it, value);
    if(ptr && ret) ptr[id] = value;
}

