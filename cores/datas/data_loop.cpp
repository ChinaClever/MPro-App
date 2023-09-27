/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_loop.h"

Data_Loop::Data_Loop()
{

}
/**
 * 设置回路开关状态
 */
void Data_Loop::loopWork()
{
    int start=0, end=0; loopNum();
    int size = mDev->cfg.nums.loopNum;  /*获取回路数量(6)*/
    //int num = mDev->cfg.nums.boardNum; if(!num) return ;
    for(int i=size; i<LOOP_NUM; ++i) mDev->cfg.nums.loopEachNum[i] = 0;     /*将每个回路数量初始化为0*/

    for(int i=0; i<size; ++i) {
        if(mDev->cfg.param.devSpec > 1) {   /*不为A系列*/
            if(i) start += mDev->cfg.nums.loopEachNum[i-1];
            end += mDev->cfg.nums.loopEachNum[i];
            loopData(i, start, end); //cout << start << end;    /*设置回路开关状态*/
        } else loopBreaker(i);
    }
}

void Data_Loop::loopNum()
{
    int size = mDev->cfg.nums.loopNum;  /*获取回路数量*/
    sObjData *obj = &(mDev->loop); obj->vol.size = size;
    obj->size = obj->relay.size = obj->cur.size = obj->pow.size = size;
    for(int i=0; i<obj->size; ++i) obj->relay.offAlarm[i] = sRelay::OffALarm;
    if(!mDev->cfg.param.isBreaker) {
        for(int i=0; i<obj->size; ++i) obj->relay.sw[i] = 2;
        obj->relay.size = 0;
    }
}


