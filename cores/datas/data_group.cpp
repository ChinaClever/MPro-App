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
    sObjData *obj = &(mDev->group); obj->vol.size = obj->cur.size =0; int size = 0;
    if(mDev->cfg.param.groupEn && mDev->output.pow.size) size = GROUP_NUM;  /*分组启用并且输出位功率不为0*/
    obj->size = obj->pow.size = size;

    if(size & mDev->output.relay.size) size = GROUP_NUM; else size = 0;
    obj->relay.size = size; //cout << obj->size << obj->pow.size << obj->relay.size;
}

QList<int> Data_Group::outletByGroup(int id, int addr)
{
    QList<int> res;
    sDevData *dev = cm::devData(addr);
    uchar *ptr = dev->cfg.nums.group[id];
    for(int i=0; i<OUTPUT_NUM; ++i) if(ptr[i]) res << i;    /*如果ptr[i]不为零，将i添加到res列表中*/

    return res; /*返回res列表，包含与指定分组相关的输出位索引*/
}
/**
 * 分组超限断电，分组定时
 */
void Data_Group::groupWork()
{
    sDevData *dev = cm::masterDev();
    for(int i=0; i<GROUP_NUM; ++i) {
        QList<int> ls;
        if(dev->group.size) ls = outletByGroup(i); // cfg.param.groupEn     /*获取与指定分组相关的输出位列表*/
        sumObjData(i, mDev->group, mDev->output, ls);   /*计算电压、电流、功率、无功功率、视在功率、电能、功率因数*/
        //cout << i << ls << mDev->group.pow.value[i];
    } setGroupSize(); groupOverrunOff(); groupTiming(); /*分组超限断电，分组定时*/
}

void Data_Group::disGroupAlarm(int id)
{
    mDev->group.vol.en[id] = 0;
    mDev->group.cur.en[id] = 0;
    mDev->group.pow.en[id] = 0;
    mDev->group.relay.disabled[id] = 0;
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
            if(cm::cn()) {
                db.event_type = QStringLiteral("超限断电");
                db.event_content = QStringLiteral("组继电器%1断开").arg(i+1);
            } else {
                db.event_type = "Overlimit power outage ";
                db.event_content = QStringLiteral("Group relay %1 disconnected ").arg(i+1);
            } Log_Core::bulid()->append(db);
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
            sRelayUnit *relay = &(cm::masterDev()->output.relay);
            foreach (const auto &i, ls){
                if(relay->disabled[i]) ls.removeOne(i);
                else if(res) relay->cnt[i] += 1;
            }

            OP_Core::bulid()->relaysCtrl(ls, res-1);
            sEventItem db; res--; //db.addr = i;
            if(cm::cn()) {
                db.event_type = QStringLiteral("定时开关");
                db.event_content = QStringLiteral("组继电器%1").arg(i+1);
                if(res) db.event_content += QStringLiteral("接通");
                else db.event_content += QStringLiteral("断开");
            } else {
                db.event_type = "time switch ";
                db.event_content = "group relay ";
                if(res) db.event_content += "put through ";
                else db.event_content += "break";
            } Log_Core::bulid()->append(db);
        }
    }
}
