/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_outlet.h"

Data_Outlet::Data_Outlet()
{

}

void Data_Outlet::outletNums()
{    
    sObjData *obj = &(mDev->output);
    int size = mDev->cfg.nums.outputNum; obj->vol.size =0;
    obj->size = obj->cur.size = obj->pow.size = obj->relay.size = size;

    switch (mDev->cfg.param.devSpec) {
    case 1: obj->size = obj->cur.size = obj->pow.size = obj->relay.size = 0;break;
    case 2: obj->relay.size = 0; break;
    case 3: obj->cur.size = obj->pow.size = 0; break;
    }
}


void Data_Outlet::outletOverrunOff()
{
    sObjData *obj = &(mDev->output);
    for(int i=0; i<obj->relay.size; ++i) {
        bool ret = relayOverrunOff(*obj, i);
        if(ret) {
            sEventItem db;
            OP_Core::bulid()->relayCtrl(i+1, 0);
            db.event_type = QStringLiteral("超限断电");
            db.event_content = QStringLiteral("继电器%1断开").arg(i+1);
            Log_Core::bulid()->append(db);
        }
    }
}

void Data_Outlet::outletTiming()
{
    sObjData *obj = &(mDev->output);
    for(int i=0; i<obj->relay.size; ++i) {
        int res = relayTiming(*obj, i);
        if(res) {
            sEventItem db; res -= 1;
            OP_Core::bulid()->relayCtrl(i+1, res);
            db.event_type = QStringLiteral("定时开关");
            db.event_content = QStringLiteral("继电器%1").arg(i+1);
            if(res) db.event_content += QStringLiteral("接通");
            else db.event_content += QStringLiteral("断开");
            Log_Core::bulid()->append(db);
        }
    }
}


void Data_Outlet::outletWork()
{
    outletNums();
    outletTiming();
    outletOverrunOff();
}

