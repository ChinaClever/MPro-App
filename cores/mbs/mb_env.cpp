/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_env.h"

Mb_Env::Mb_Env(QObject *parent) : Mb_Loop{parent}
{

}


void Mb_Env::mbEnvUpdate()
{
    upEnvData();
    upEnvThreshold();
}

void Mb_Env::upEnvData()
{
    sEnvData *obj = &(mDevData->env);
    vshort vs; int size = obj->size;
    for(int i=0; i<18; ++i) vs << 0xFFFF;
    for(int i=0; i<size; ++i) {
        vs[2*i] = obj->tem.value[i];
        vs[2*i+1] = obj->hum.value[i];
    }

    setRegs(MbReg_Envs, vs);
}

void Mb_Env::upEnvThreshold()
{
    vshort vs; sEnvData *env = &(mDevData->env);
    for(int i=0; i<env->size; ++i)
        vs << env->tem.max[i] << env->tem.min[i];
    setRegs(MbReg_SetEnv, vs); vs.clear();

    for(int i=0; i<env->size; ++i)
        vs << env->hum.max[i] << env->hum.min[i];
    setRegs(MbReg_SetEnv+16, vs);
}
