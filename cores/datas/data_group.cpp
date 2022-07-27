/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_group.h"

Data_Group::Data_Group() : Data_Line{}
{

}

void Data_Group::setGroupSize()
{
    int size = 0;
    if(mDev->cfg.nums.groupEn) size = GROUP_NUM;
    sObjData *obj = &(mDev->group); obj->vol.size = size;  ///////====   0
    obj->size = obj->relay.size = obj->cur.size = obj->pow.size = size;
    //for(int i=0; i<obj->size; ++i) obj->relay.mode[i] = sRelay::NoAlarm;
}

QList<int> Data_Group::outletByGroup(int id, int addr)
{
    QList<int> res;
    sDevData *dev = cm::devData(addr);
    if(dev->cfg.nums.groupEn) {
        uchar *ptr = dev->cfg.nums.group[id];
        for(int i=0; i<OUTPUT_NUM; ++i) if(ptr[i]) res << i;
    } else disGroupAlarm(id);

    return res;
}

void Data_Group::groupWork()
{
    for(int i=0; i<GROUP_NUM; ++i) {
        QList<int> ls = outletByGroup(i);
        sumObjData(i, mDev->group, mDev->output, ls);
    } setGroupSize();
}

void Data_Group::disGroupAlarm(int id)
{
    mDev->group.vol.en[id] = 0;
    mDev->group.cur.en[id] = 0;
    mDev->group.pow.en[id] = 0;
    mDev->group.relay.mode[id] = 0;
}
