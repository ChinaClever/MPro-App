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
        if(mDev->env.isInsert[i]) size = 2; /*检测到插入传感器*/
    } if(mDev->cfg.param.sensorBoxEn) size = SENOR_NUM; /*传感器盒子启用时，size为4*/
    mDev->env.size = mDev->env.tem.size = mDev->env.hum.size = size;
}

void Data_Env::initData()
{
    for(int i=0; i<SENOR_NUM; ++i) {
        if(!mDev->env.isInsert[i]) {
            mDev->env.tem.value[i] = 0 * COM_RATE_TEM; // 0xffff
            mDev->env.hum.value[i] = 0 * COM_RATE_HUM;
        }
    }
}


void Data_Env::envWork()
{
    setEnvSize();   /*设置温湿度数据的个数*/
    initData();     /*初始化温湿度的值*/
}
