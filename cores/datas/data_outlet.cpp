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
    sObjData *obj = &(mDev->output); int size = 0; obj->vol.size =0;
    for(uint i=0; i<mDev->cfg.nums.boardNum; ++i) size += mDev->cfg.nums.boards[i];
    obj->size = obj->cur.size = obj->pow.size = obj->relay.size = size;
}


void Data_Outlet::outletOverrunOff()
{
    sObjData *obj = &(mDev->output);
    for(int i=0; i<obj->relay.size; ++i) {
        bool ret = relayOverrunOff(*obj, i);
        if(ret) {
            OP_Core::bulid()->relayCtrl(i+1, 0);
            sOpItem db;
            db.op_src = QStringLiteral("超限断电");
            db.content = QStringLiteral("继电器%1断开").arg(i+1);
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
            OP_Core::bulid()->relayCtrl(i+1, res-1);
            sOpItem db;
            db.op_src = QStringLiteral("定时开关"); res--;
            db.content = QStringLiteral("继电器%1").arg(i+1);
            if(res) db.content += QStringLiteral("接通");
            else db.content += QStringLiteral("断开");
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
