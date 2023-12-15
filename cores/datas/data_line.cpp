/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_line.h"

Data_Line::Data_Line()
{

}

void Data_Line::lineWork()
{
    int size = mDev->cfg.nums.lineNum; if(size == 0) size = 1;  /*获取设备单三相*/
    if(mDev->cfg.nums.loopNum) {    /*回路数量不为0*/
        int num = mDev->cfg.nums.loopNum / size;
        for(int i=0; i<size; ++i) {
            int start = i * num;
            int end = (i+1) * num;
            lineData(i, start, end);    /*计算电压、电流、功率、无功功率、视在功率、电能、功率因数*/
        }
    } else {
        int num = mDev->cfg.nums.outputNum / size;
        for(int i=0; i<size; ++i) {
            int start = i * num;
            int end = (i+1) * num;
            lineDataNoLoop(i, start, end);  /*计算电压、电流、功率、无功功率、视在功率、电能、功率因数*/
        }
    }inletNum(); lineVoltage(); /*获取相电压的值*/
}

void Data_Line::inletNum()
{
    int size = mDev->cfg.nums.lineNum;
    sObjData *obj = &(mDev->line); obj->relay.size = 0;
    obj->size = obj->vol.size = obj->cur.size = obj->pow.size = size;
}

void Data_Line::lineVoltage()
{
    sObjData *obj = &(mDev->line); //obj->relay.size = 0;
    for (int i = 0; i < obj->size; ++i) {
        obj->lineVol[i] = obj->vol.value[i] * std::sqrt(3); /*将相电压的当前值的根号3倍给线电压*/
    }
}
/**
 * 产品实时频率、电压频率。电压、电流、功率、无功功率、视在功率、电能、功率因数写入统计数据结构体中
 */
void Data_Line::tgWork()
{
    sObjData *obj = &(mDev->line);
    sTgObjData *tg = &(mDev->tg);
    int size = obj->size; calHz();
    if(!size) {
        obj = &(mDev->output);
        size = obj->size;
    } if(!size) return ;

    tg->vol.value = averageValue(obj->vol.value, 0, size);
    tg->cur.value = summation(obj->cur.value, 0, size);
    tg->pow.value = summation(obj->pow.value, 0, size);
    tg->reactivePow = summation(obj->reactivePow, 0, size);
    tg->artPow = summation(obj->artPow, 0, size);
    tg->ele = summation(obj->ele, 0, size);

    if(tg->artPow) {
        tg->pf = (tg->pow.value*COM_RATE_PF)/tg->artPow;
        if(tg->pf > 99) tg->pf = 99;
    } else tg->pf = 0;
}

void Data_Line::calHz()
{
    for(uint i=0; i<mDev->cfg.nums.boardNum; ++i) {
        uchar hz = mDev->rtu.hzs[i];
        if(hz) {mDev->hz = mDev->cfg.param.hz = hz;break;}
    }
}
