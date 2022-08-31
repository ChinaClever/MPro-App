/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_object.h"
#include "log_core.h"
#define FAULT_NUM 15

OP_Object::OP_Object(QObject *parent) : SerialPort{parent}
{
    mOpData = new sOpIt;
    memset(mOpData, 0, sizeof(sOpIt));
    mDev = cm::masterDev();
}

bool OP_Object::dataFiltering(uint &dest, uint &src, uint max, uint min)
{
    bool ret = true;
    if(mDev->cfg.param.runTime < 48*60) dest = src;
    if((src >= min) && (src < max)) {
        dest = src; ret = true;
    } else {
        ret = false;
    }

    return ret;
}

void OP_Object::faultLog(int id, uint *cnt, uint value)
{
    uint num = FAULT_NUM;
    uint *dtc = mDev->dtc.code;
    if(mDev->cfg.param.runTime < 48*60) num = 1;
    if((cnt[id] == num) && dtc[id]) {
        sHardwareItem it;
        if(dtc[id] & FaultCode::DTC_VOL) {
            if(value == 16*COM_RATE_VOL) {
                it.module = tr("Chip fault");
                it.content = tr("计量芯片出错 %1").arg(id+1);
            } else {
                it.module = tr("Vol fault");
                it.content = tr("Output %1 fault VOL=%2V").arg(id+1).arg(value/COM_RATE_VOL);
            }
        } else if(dtc[id] & FaultCode::DTC_CUR){
            it.module = tr("Cur fault");
            it.content = tr("Output %1 fault CUR=%2A").arg(id+1).arg(value/COM_RATE_CUR);
        } else if(dtc[id] & FaultCode::DTC_ELE) {
            it.module = tr("ele fault");
            it.content = tr("Output %1 fault ELE:%2Kwh, %3Kwh").arg(id+1)
                    .arg(mDev->output.ele[id]/COM_RATE_ELE).arg(value/COM_RATE_ELE);
        } else cout << dtc[id];

        Log_Core::bulid(this)->append(it);
    }
}

void OP_Object::faultCode(int id, bool f, uint *cnt, FaultCode code)
{
    uint *dtc = mDev->dtc.code;
    if(f) {
        cnt[id] += 1;
        dtc[id] |= code;
        mDev->dtc.fault = 1;
    } else {
        cnt[id] = 0;
        dtc[id] &= ~code;
    }
}

bool OP_Object::volFaultCheck(uchar k, uchar i)
{
    int id = k + i, min = 0;
    uint *src = mOpData->vol;
    uint *cnt = mDev->dtc.cnt[0];
    uint *dest = mDev->output.vol.value;
    if(!mDev->cfg.param.isBreaker) min = 80;
    bool ret = dataFiltering(dest[id], src[i], COM_MAX_VOL, min);
    if(src[i] == 16*COM_RATE_VOL) {ret = false; dest[id] = 0;}
    faultCode(id, ret, cnt, FaultCode::DTC_VOL);
    if(!ret) {
        faultLog(id, cnt, src[i]);
        if(cnt[id] == FAULT_NUM) dest[id] = 222*COM_RATE_VOL;
    }
    return ret;
}

bool OP_Object::curFaultCheck(uchar k, uchar i)
{
    int id = k + i;
    uint *src = mOpData->vol;
    uint *cnt = mDev->dtc.cnt[1];
    uint *dest = mDev->output.cur.value;
    bool ret = dataFiltering(dest[id], src[i], COM_MAX_CUR);
    faultCode(id, ret, cnt, FaultCode::DTC_CUR);
    if(!ret) {
        faultLog(id, cnt, src[i]);
        if(cnt[id] == FAULT_NUM) dest[id] = 0;
    }
    return ret;
}

void OP_Object::powFaultCheck(uchar k, uchar i)
{
    int id = k + i;
    uint value = mOpData->pf[i];
    sObjData *obj = &mDev->output;
    if(value < 100) {
        obj->pf[id] = value;
        obj->artPow[id] = obj->vol.value[id] * obj->cur.value[id] / 100.0;
        obj->pow.value[id] = obj->artPow[id] * obj->pf[id] / 100.0;
        obj->reactivePow[id] = obj->artPow[id] - obj->pow.value[id];
    } else {
        cout << value/COM_RATE_PF;
        sSysItem it; it.module = tr("PF");
        it.content = tr("输出位 %1 功率因素错误％2").arg(id+1).arg(value/COM_RATE_PF);
        Log_Core::bulid(this)->append(it);
    }
}


void OP_Object::eleFaultCheck(uchar k, uchar i)
{
    int id = k + i;
    bool ret = true;
    uint *src = mOpData->ele;
    uint *cnt = mDev->dtc.cnt[2];
    uint *dest = mDev->output.ele;

    if(dest[id] && src[i]) {
        if(src[i] - dest[id] > 1) {
            ret = false;
            faultLog(id, cnt, src[i]);
        } else cnt[id] = 0;
    }

    dest[id] = src[i];
    faultCode(id, ret, cnt, FaultCode::DTC_ELE);
}

void OP_Object::fillData(uchar addr)
{
    sDevData *dev = mDev; uchar k = 0;
    sOpIt *it = mOpData; mDev->dtc.fault = 0;
    dev->cfg.nums.boards[addr] = it->size; addr -= 1;
    for(int i=0; i<addr; ++i) k += dev->cfg.nums.boards[i];
    for(int i=0; i<it->size; ++i) {
        volFaultCheck(k, i);
        curFaultCheck(k, i);
        powFaultCheck(k, i);
        eleFaultCheck(k, i);
        dev->output.relay.sw[k+i] = it->sw[i];
    }

    dev->offLine = 3;
    dev->rtu.hzs[addr] = it->hz;
    dev->cfg.nums.boards[addr] = it->size;
    dev->cfg.vers.opVers[addr] = it->version;
    dev->rtu.chipStates[addr] = it->chipStatus;
    for(int i=0; i<4; ++i) dev->rtu.offLines[i] = it->ens[i];
}
