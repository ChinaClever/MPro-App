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
    if(mDev->cfg.param.groupEn) size = GROUP_NUM;
    sObjData *obj = &(mDev->group);
    obj->vol.size = obj->cur.size =0;
    obj->size = obj->pow.size = size;
    if(size & mDev->output.relay.size) size = GROUP_NUM; else size = 0;
    obj->relay.size = size;
}

QList<int> Data_Group::outletByGroup(int id, int addr)
{
    QList<int> res;
    sDevData *dev = cm::devData(addr);
    uchar *ptr = dev->cfg.nums.group[id];
    for(int i=0; i<OUTPUT_NUM; ++i) if(ptr[i]) res << i;

    return res;
}

void Data_Group::groupWork()
{
    sDevData *dev = cm::masterDev();
    for(int i=0; i<GROUP_NUM; ++i) {
        QList<int> ls;
        if(dev->cfg.param.groupEn) ls = outletByGroup(i);
        sumObjData(i, mDev->group, mDev->output, ls);
        //cout << i << ls << mDev->group.pow.value[i];
    } setGroupSize(); groupOverrunOff(); groupTiming();
}

void Data_Group::disGroupAlarm(int id)
{
    mDev->group.vol.en[id] = 0;
    mDev->group.cur.en[id] = 0;
    mDev->group.pow.en[id] = 0;
    mDev->group.relay.en[id] = 0;
    mDev->group.relay.offAlarm[id] = 0;
    mDev->group.relay.timingEn[id] = 0;
    mDev->group.relay.overrunOff[id] = 0;
}

void Data_Group::groupOverrunOff()
{
    sObjData *obj = &(mDev->group);
    for(int i=0; i<obj->relay.size; ++i) {
        bool ret = relayOverrunOff(*obj, i);
        if(ret) {
            QList<int> ls = outletByGroup(i);
            OP_Core::bulid()->relaysCtrl(ls, 0);
            sEventItem db; //db.addr = i;
            db.event_type = QStringLiteral("超限断电");
            db.event_content =QStringLiteral("组继电器%1断开").arg(i+1);
            Log_Core::bulid()->append(db);
        }
    }
}

void Data_Group::groupTiming()
{
    sObjData *obj = &(mDev->group);
    for(int i=0; i<obj->relay.size; ++i) {
        int res = relayTiming(*obj, i);
        if(res) {
            QList<int> ls = outletByGroup(i);
            OP_Core::bulid()->relaysCtrl(ls, res-1);
            sEventItem db; //db.addr = i;
            db.event_type = QStringLiteral("定时开关"); res--;
            db.event_content = QStringLiteral("组继电器%1").arg(i+1);
            if(res) db.event_content += QStringLiteral("接通");
            else db.event_content += QStringLiteral("断开");
            Log_Core::bulid()->append(db);
        }
    }
}
