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
    int size = mDev->cfg.nums.loopNum;
    for(int i=0; i<size; ++i) {
        int start = mDev->cfg.nums.loopStarts[i];
        int end = mDev->cfg.nums.loopEnds[i];
        loopData(i, start, end);
    } loopNum();
}

void Data_Loop::loopNum()
{
    int size = mDev->cfg.nums.loopNum;
    sObjData *obj = &(mDev->loop); obj->vol.size = size;  ///////====   0
    obj->size = obj->relay.size = obj->cur.size = obj->pow.size = size;
    for(int i=0; i<obj->size; ++i) obj->relay.mode[i] = sRelay::OffALarm;
}


