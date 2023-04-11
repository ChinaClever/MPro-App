/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_env.h"

Data_Env::Data_Env()
{

}


void Data_Env::setEnvSize()
{
    int size = 4;
    for(int i=0; i<SENOR_NUM; ++i) {
        if(mDev->env.isInsert[i]) size = 2;
    } if(mDev->cfg.param.sensorBoxEn) size = SENOR_NUM;
    mDev->env.size = mDev->env.tem.size = mDev->env.hum.size = size;
}

void Data_Env::initData()
{
    for(int i=0; i<SENOR_NUM; ++i) {
        if(!mDev->env.isInsert[i]) {
            mDev->env.tem.value[i] = 0xffff * COM_RATE_TEM;
            mDev->env.hum.value[i] = 0xffff * COM_RATE_HUM;
        }
    }
}


void Data_Env::envWork()
{
    setEnvSize();
    initData();
}
