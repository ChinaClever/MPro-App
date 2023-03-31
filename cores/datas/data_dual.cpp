/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_dual.h"

Data_Dual::Data_Dual()
{

}

int Data_Dual::setDualSize(int id, int type)
{
    int size = 0;
    sObjData *obj = nullptr;
    switch (type) {
    case DType::Dual:
        size = mDev->cfg.nums.outputNum;
        obj =  &(cm::devData(id)->dual);
        break;

//    case DType::CabLine:
//        size = mDev->cfg.nums.lineNum;
//        obj =  &(cm::devData(id)->cabLoop);
//        break;

//    case DType::CabLoop:
//        size = mDev->cfg.nums.loopNum;
//        obj =  &(cm::devData(id)->cabLoop);
//        break;
    }

    if(obj) {
        obj->vol.size = obj->cur.size = obj->relay.size =0;
        obj->size = obj->pow.size = size;
    }

    return size;
}


void Data_Dual::dualWork()
{
    if(mDev->cfg.param.devMode == EDevMode::DM_Dual) {
        dualWorkdown(DType::Dual);
        //dualWorkdown(DType::CabLine);
        //dualWorkdown(DType::CabLoop);
    }
}

void Data_Dual::dualWorkdown(int type)
{
    setDualSize(0, type);
    int num = mDev->cfg.nums.slaveNum;
    for (int i=0; i <= num; i += 2) {
        int size = setDualSize(i, type);
        if(size > 0) dualData(i, type);
        //else disDualAlarm();
    } //dualTiming(0);
}


void Data_Dual::dualData(int id, int type)
{
    sObjData *dest = nullptr;
    sObjData *src1 = nullptr;
    sObjData *src2 = nullptr;

    switch (type) {
    case DType::Dual:
        dest = &(cm::devData(id)->dual);
        src1 = &(cm::devData(id)->output);
        src2 = &(cm::devData(id+1)->output);
        break;

//    case DType::CabLine:
//        dest = &(cm::devData(id)->cabLine);
//        src1 = &(cm::devData(id)->line);
//        src2 = &(cm::devData(id+1)->line);
//        break;

//    case DType::CabLoop:
//        dest = &(cm::devData(id)->cabLoop);
//        src1 = &(cm::devData(id)->loop);
//        src2 = &(cm::devData(id+1)->loop);
//        break;
    }

    for(int i=0; i<dest->size; ++i) {
        dest->vol.value[i] = (src1->vol.value[i] + src2->vol.value[i])/2;
        dest->cur.value[i] = src1->cur.value[i] + src2->cur.value[i];
        dest->pow.value[i] = src1->pow.value[i] + src2->pow.value[i];
        dest->pow.rated[i] = src1->pow.rated[i] + src2->pow.rated[i];
        dest->reactivePow[i] = src1->reactivePow[i] + src2->reactivePow[i];
        dest->artPow[i] = src1->artPow[i] + src2->artPow[i];
        dest->ele[i] = src1->ele[i] + src2->ele[i];
        dest->pf[i] = calPf(i, *dest);
    } //dualTiming(id);
}


void Data_Dual::dualTiming(int id)
{
    sObjData *obj = &(cm::devData(id)->dual); // &(mDev->dual);
    for(int i=1; i<=obj->relay.size; ++i) {
        int res = relayTiming(*obj, i);
        if(res) {
            sEventItem db; res -=1; //db.addr = id;
            OP_Core::bulid()->relayCtrl(i, res);
            db.event_type = QStringLiteral("机架定时开关");
            db.event_content = QStringLiteral("继电器%1").arg(i);
            if(res) db.event_content += QStringLiteral("接通");
            else db.event_content += QStringLiteral("断开");
            Log_Core::bulid()->append(db);
        }
    }
}

void Data_Dual::disDualAlarm()
{
    sObjData *it = &mDev->dual;
    for(int i=0; i<OUTPUT_NUM; ++i) {
        it->vol.en[i] = 0;
        it->cur.en[i] = 0;
        it->pow.en[i] = 0;
        it->relay.disabled[i] = 0;
        it->relay.offAlarm[i] = 0;
        it->relay.timingEn[i] = 0;
        it->relay.overrunOff[i] = 0;
    }
}
