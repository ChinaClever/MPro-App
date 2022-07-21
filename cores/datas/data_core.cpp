/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_core.h"
#include "alarm_Updater.h"

Data_Core::Data_Core()
{

}

Data_Core *Data_Core::bulid()
{
    static Data_Core* sington = nullptr;
    if(sington == nullptr) sington = new Data_Core();
    return sington;
}

void Data_Core::upAlarm()
{
    static uint cnt = 1;
    if(!(++cnt % 5)) {
        Alarm_Updater::bulid()->run();
    }
}

void Data_Core::outletNums()
{
    sObjData *obj = &(mDev->output); int size = 0; obj->vol.size =0;
    for(uint i=0; i<mDev->info.opNum; ++i) size += mDev->info.ops[i];
    obj->size = obj->cur.size = obj->pow.size = obj->relay.size = size;
}

void Data_Core::run()
{
    while(isRun) {
        outletNums();
        cm::mdelay(1100);tgWork();
        groupWork(); cm::mdelay(100);
        loopWork(); cm::mdelay(100);
        lineWork(); cm::mdelay(100);
        upAlarm(); cm::mdelay(100);
    }
}
