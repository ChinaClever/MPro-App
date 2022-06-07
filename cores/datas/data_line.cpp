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
    int size = mDev->info.lineNum;
    if(size == 0) size = 1;
    int num = mDev->info.loopNum / size;
    for(int i=0; i<size; ++i) {
        int start = i * num;
        int end = (i+1) * num;
        lineData(i, start, end);
    }
}

void Data_Line::inletNum()
{
    int size = mDev->info.lineNum;
    sObjData *obj = &(mDev->line); obj->relay.size = 0;
    obj->size = obj->vol.size = obj->cur.size = obj->pow.size = size;
}

void Data_Line::tgWork()
{
    sObjData *obj = &(mDev->output);
    sTgObjData *tg = &(mDev->tg);
    int size = obj->size; calHz();
    inletNum();

    tg->vol.value = averageValue(obj->vol.value, 0, size);
    tg->cur.value = summation(obj->cur.value, 0, size);
    tg->pow.value = summation(obj->pow.value, 0, size);
    tg->reactivePow = summation(obj->reactivePow, 0, size);
    tg->artPow = summation(obj->artPow, 0, size);
    tg->ele = summation(obj->ele, 0, size);

    if(tg->artPow) {
        tg->pf = tg->pow.value * 100.0 / tg->artPow;
        if(tg->pf > 99) tg->pf = 99;
    } else tg->pf = 0;
}

void Data_Line::calHz()
{
    for(uint i=0; i<mDev->info.loopNum; ++i) {
        uchar hz = mDev->info.hzs[i];
        if(hz) {mDev->hz = mDev->info.hz = hz; break;}
    }
}
