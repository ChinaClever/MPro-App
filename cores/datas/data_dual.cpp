/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_dual.h"

Data_Dual::Data_Dual()
{

}

int Data_Dual::setDualSize(int id)
{
    int size = 0;
    if(mDev->cfg.param.devMode == DevMode::DM_Dual) {
        size = mDev->cfg.nums.outputNum;
        //mDev->cfg.nums.slaveNum = 1;
    }

    sObjData *obj =  &(cm::devData(id)->dual); // &(mDev->dual);
    obj->vol.size = obj->cur.size = 0;
    obj->size = obj->pow.size = size;
    if(size & mDev->output.relay.size) size = mDev->cfg.nums.outputNum; else size = 0;
    obj->relay.size = size;
    return obj->size;
}


void Data_Dual::dualWork()
{
    if(mDev->cfg.param.cascadeAddr) {dualTiming(0); return;}
    if(mDev->cfg.param.devMode == DevMode::DM_Dual) {
        int num = mDev->cfg.nums.slaveNum;
        for (int i=0; i <= num; i += 2) {
            int size = setDualSize(i);
            if(size > 0) dualData(i);
            //else disDualAlarm();
        }
    }
}


void Data_Dual::dualTiming(int id)
{
    sObjData *obj = &(cm::devData(id)->dual); //&(mDev->dual);
    for(int i=0; i<obj->relay.size; ++i) {
        int res = relayTiming(*obj, i);
        if(res) {
            OP_Core::bulid()->relayCtrl(i+1, res-1);
            sEventItem db; db.addr = id;
            db.event_type = QStringLiteral("机架定时开关"); res--;
            db.event_content = QStringLiteral("继电器%1").arg(i+1);
            if(res) db.event_content += QStringLiteral("接通");
            else db.event_content += QStringLiteral("断开");
            Log_Core::bulid()->append(db);
        }
    }
}

void Data_Dual::dualData(int id)
{
    sObjData *dest = &(cm::devData(id)->dual);
    sObjData *src1 = &(cm::devData(id)->output);
    sObjData *src2 = &(cm::devData(id+1)->dual);
    for(int i=0; i<dest->size; ++i) {
        dest->vol.value[i] = (src1->vol.value[i] + src2->vol.value[i])/2;
        dest->cur.value[i] = src1->cur.value[i] + src2->cur.value[i];
        dest->pow.value[i] = src1->pow.value[i] + src2->pow.value[i];
        dest->reactivePow[i] = src1->reactivePow[i] + src2->reactivePow[i];
        dest->artPow[i] = src1->artPow[i] + src2->artPow[i];
        dest->ele[i] = src1->ele[i] + src2->ele[i];
        dest->pf[i] = calPf(i, *dest);
    } dualTiming(id);
}

void Data_Dual::disDualAlarm()
{
    sObjData *it = &mDev->dual;
    for(int i=0; i<OUTPUT_NUM; ++i) {
        it->vol.en[i] = 0;
        it->cur.en[i] = 0;
        it->pow.en[i] = 0;
        it->relay.en[i] = 0;
        it->relay.offAlarm[i] = 0;
        it->relay.timingEn[i] = 0;
        it->relay.overrunOff[i] = 0;
    }
}
