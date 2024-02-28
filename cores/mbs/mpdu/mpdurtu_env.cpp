/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mpdurtu_env.h"

Mpdurtu_Env::Mpdurtu_Env(QObject *parent) : Mpdurtu_Group{parent}
{

}

void Mpdurtu_Env::env_BaseUpdate(sEnvData *obj , uint v[],int startReg)
{
    vshort vs; int size = SENOR_NUM;
    for(int i=0; i<size; ++i) {
        if(obj->isInsert[i]) {
            vs << v[i];
        } else vs << 0;
    }
    setRegs(startReg, vs);
}

void Mpdurtu_Env::env_BaseUpdateAlram(uint v[],int startReg)
{
    vshort vs; int size = SENOR_NUM;
    for(int i=0; i<size; ++i) {
        vs << v[i];
    }
    setRegs(startReg, vs);
}


void Mpdurtu_Env::env_Update()
{
    sEnvData *obj = &(mDevData->env);
    env_BaseUpdate(obj , obj->tem.value , MReg_Tem);
    env_BaseUpdateAlram(obj->tem.min , MReg_TemMin);
    env_BaseUpdateAlram(obj->tem.max , MReg_TemMax);
    env_BaseUpdateAlram(obj->tem.crMin , MReg_TemCrMin);
    env_BaseUpdateAlram(obj->tem.crMax , MReg_TemCrMax);
    env_BaseUpdate(obj , obj->hum.value , MReg_Hum);
    env_BaseUpdateAlram(obj->hum.min , MReg_HumMin);
    env_BaseUpdateAlram(obj->hum.max , MReg_HumMax);
    env_BaseUpdateAlram(obj->hum.crMin , MReg_HumCrMin);
    env_BaseUpdateAlram(obj->hum.crMax , MReg_HumCrMax);
    vshort vs;
    vs << obj->door[0];
    vs << obj->door[1];
    setRegs(MReg_Door, vs);
    vs << obj->water[0];
    setRegs(MReg_Water, vs);
    vs << obj->smoke[0];
    setRegs(MReg_Smoke, vs);
}

void Mpdurtu_Env::env_setting(ushort addr, ushort address, ushort value)
{
    ushort reg = 1;
    sEnvData *obj = &(cm::devData(addr)->env);
    sAlarmUnit *unit = nullptr; uint *ptr = nullptr;
    int id = 0; sDataItem it;
    it.type = DType::Env; it.addr = addr;

    if( address >= MReg_TemMin && address < MReg_Hum ){
        unit = &(obj->tem); it.topic = DTopic::Tem;
    }else{
        unit = &(obj->hum); it.topic = DTopic::Hum;
    }

    if((address >= MReg_TemMax && address < MReg_TemCrMin ) || (address >= MReg_HumMax && address < MReg_HumCrMin)){
        if(it.topic == DTopic::Tem ){ id = address - MReg_TemMax; }
        else {id = address - MReg_HumMax;}
        reg = 1;
        ptr = unit->max; it.subtopic = DSub::VMax;
    }else if( (address >= MReg_TemCrMax && address < MReg_Hum ) || (address >= MReg_HumCrMax && address < MReg_Door)){
        if(it.topic == DTopic::Tem ){ id = address - MReg_TemCrMax; }
        else {id = address - MReg_HumCrMax;}
        reg = 2;
        ptr = unit->crMax; it.subtopic = DSub::VCrMax;
    }else if( (address >= MReg_TemCrMin && address < MReg_TemCrMax ) || (address >= MReg_HumCrMin && address < MReg_HumCrMax)){
        if(it.topic == DTopic::Tem ){ id = address - MReg_TemCrMin; }
        else {id = address - MReg_HumCrMin;}
        reg = 3;
        ptr = unit->crMin; it.subtopic = DSub::VCrMin;
    }else if( (address >= MReg_TemMin && address < MReg_TemMax ) || (address >= MReg_HumMin && address < MReg_HumMax)){
        if(it.topic == DTopic::Tem ){ id = address - MReg_TemMin; }
        else {id = address - MReg_HumMin;}
        reg = 4;
        ptr = unit->min; it.subtopic = DSub::VMin;
    }

    it.id = id+1;

    bool ret = alarmUnitCheck(reg, id, unit, value);
    if(ptr && ret && ptr[id] != value) setting(it, value);
    if(ptr && ret) ptr[id] = value;
}
