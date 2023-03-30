/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_loop.h"

Data_Loop::Data_Loop()
{

}

void Data_Loop::loopWork()
{
    int num = mDev->cfg.nums.boardNum;
    loopNum(); if(!num) return ;
    int size = mDev->cfg.nums.loopNum;
    int start=0, end=0;

    for(int i=0; i<size; ++i) {
        //int start = mDev->cfg.nums.loopStarts[i]-1;
        //int end = mDev->cfg.nums.loopEnds[i];

        if(i) start += mDev->cfg.nums.loopEachNum[i-1];
        end += mDev->cfg.nums.loopEachNum[i];
        loopData(i, start, end); //cout << start << end;
    }
}

void Data_Loop::loopNum()
{
    int size = mDev->cfg.nums.loopNum;
    sObjData *obj = &(mDev->loop); obj->vol.size = size;
    obj->size = obj->relay.size = obj->cur.size = obj->pow.size = size;
    for(int i=0; i<obj->size; ++i) obj->relay.offAlarm[i] = sRelay::OffALarm;
    if(!mDev->cfg.param.isBreaker) {
        for(int i=0; i<obj->size; ++i) obj->relay.sw[i] = 2;
        obj->relay.size = 0;
    }
}


