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
    sDevData *dev = cm::devData(id);

    switch (type) {
    case DType::Dual:
        size = dev->output.pow.size;
        obj =  &(cm::devData(id)->dual);
        break;

    case DType::CabLine:
        size = dev->cfg.nums.lineNum;
        obj =  &(cm::devData(id)->cabLoop);
        break;

    case DType::CabLoop:
        size = dev->cfg.nums.loopNum;
        obj =  &(cm::devData(id)->cabLoop);
        break;
    }

    if(obj) {
        obj->vol.size = obj->cur.size = obj->relay.size =0;
        obj->size = obj->pow.size = size;
    }

    return size;
}


void Data_Dual::dualWork()
{
    int mode = mDev->cfg.param.devMode;
    int addr = mDev->cfg.param.cascadeAddr;
    if((mode == EDevMode::DM_Dual) && (addr == 0)) {
        dualWorkdown(DType::Dual);
        //dualWorkdown(DType::CabLine);
        //dualWorkdown(DType::CabLoop);
    } else disDualAlarm();
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

void Data_Dual::dualTgWork(int id)
{
    sTgObjData *dest = &(cm::devData(id)->cabTg);
    sTgObjData *src1 = &(cm::devData(id)->tg);
    sTgObjData *src2 = &(cm::devData(id+1)->tg);

    dest->vol.value = (src1->vol.value + src2->vol.value)/2;
    dest->cur.value = src1->cur.value + src2->cur.value;
    dest->pow.value = src1->pow.value + src2->pow.value;
    dest->pow.rated = src1->pow.rated + src2->pow.rated;
    dest->reactivePow = src1->reactivePow + src2->reactivePow;
    dest->artPow = src1->artPow + src2->artPow;
    dest->ele = src1->ele + src2->ele;
    //dest->pf[i] = calPf(i, *dest);
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

    case DType::CabLine:
        dest = &(cm::devData(id)->cabLine);
        src1 = &(cm::devData(id)->line);
        src2 = &(cm::devData(id+1)->line);
        break;

    case DType::CabLoop:
        dest = &(cm::devData(id)->cabLoop);
        src1 = &(cm::devData(id)->loop);
        src2 = &(cm::devData(id+1)->loop);
        break;
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
    } dualTgWork(id); //dualTiming(id);
}


void Data_Dual::dualTiming(int id)
{
    sObjData *obj = &(cm::devData(id)->dual); // &(mDev->dual);
    for(int i=1; i<=obj->relay.size; ++i) {
        int res = relayTiming(*obj, i);
        if(res) {
            sEventItem db; res -=1; //db.addr = id;
            OP_Core::bulid()->relayCtrl(i, res);            
            if(cm::cn()) {
                db.event_type = QStringLiteral("机架定时开关");
                db.event_content = QStringLiteral("继电器%1").arg(i);
                if(res) db.event_content += QStringLiteral("接通");
                else db.event_content += QStringLiteral("断开");
            } else {
                db.event_type = "rack timing switch ";
                db.event_content = QStringLiteral("relay %1 ").arg(i);
                if(res) db.event_content += "put through ";
                else db.event_content += "break ";
            } Log_Core::bulid()->append(db);
        }
    }
}

void Data_Dual::disDualAlarm()
{
    int num = mDev->cfg.nums.slaveNum;
    for(int i=0; i<=num; ++i) {
        sDevData *dev = cm::devData(i);
        dev->dual.size = 0;
        dev->dual.pow.size = 0;

        dev->cabLoop.size = 0;
        dev->cabLoop.pow.size = 0;

        dev->cabLine.size = 0;
        dev->cabLine.pow.size = 0;
    }


    //    sObjData *it = &mDev->dual;
    //    for(int i=0; i<OUTPUT_NUM; ++i) {
    //        it->vol.en[i] = 0;
    //        it->cur.en[i] = 0;
    //        it->pow.en[i] = 0;
    //        it->relay.disabled[i] = 0;
    //        it->relay.offAlarm[i] = 0;
    //        it->relay.timingEn[i] = 0;
    //        it->relay.overrunOff[i] = 0;
    //    }
}
