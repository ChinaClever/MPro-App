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
    mDev = cm::masterDev();
    memset(mOpData, 0, sizeof(sOpIt));
    //for(int i=0; i<PACK_ARRAY_SIZE; ++i) m_swCnt[i] =1;
}

bool OP_Object::dataFiltering(uint &dest, uint &src, uint max, uint min)
{
    bool ret = true;
    if(cm::runTime() < 48*60*60) dest = src;
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
    if(cm::runTime() < 48*60*60) num = 1;
    if((cnt[id] == num) && dtc[id]) {
        sEventItem it;
        if(dtc[id] & FaultCode::DTC_VOL) {
            if(value == 16*COM_RATE_VOL) {
                it.event_type = tr("Chip fault");
                it.event_content = tr("Metering chip error %1").arg(id+1);
            } else {
                it.event_type = tr("Vol fault");
                it.event_content = tr("Output %1 fault VOL=%2V").arg(id+1).arg(value/COM_RATE_VOL);
            }
        } else if(dtc[id] & FaultCode::DTC_CUR){
            it.event_type = tr("Cur fault");
            it.event_content = tr("Output %1 fault CUR=%2A").arg(id+1).arg(value/COM_RATE_CUR);
        } else if(dtc[id] & FaultCode::DTC_ELE) {
            it.event_type = tr("ele fault");
            it.event_content = tr("Output %1 fault ELE:%2Kwh, %3Kwh").arg(id+1)
                    .arg(mDev->output.ele[id]/COM_RATE_ELE).arg(value/COM_RATE_ELE);
        } else cout << dtc[id];
        Log_Core::bulid()->append(it);
    }
}

void OP_Object::recoveryLog(int id, uint *cnt)
{
    uint num = FAULT_NUM;
    uint *dtc = mDev->dtc.code;
    if(cm::runTime() < 48*60*60) num = 1;
    if((cnt[id] > num) && dtc[id]) {
        sEventItem it;
        if(dtc[id] & FaultCode::DTC_VOL) {
            it.event_type = tr("Vol recovery");
            it.event_content = tr("Output %1 recovery VOL=%2V").arg(id+1)
                    .arg(mDev->output.vol.value[id]/COM_RATE_VOL);
        } else if(dtc[id] & FaultCode::DTC_CUR){
            it.event_type = tr("Cur recovery");
            it.event_content = tr("Output %1 recovery CUR=%2A").arg(id+1)
                    .arg(mDev->output.vol.value[id]/COM_RATE_CUR);;
        }  else {cout << dtc[id]; return ;}
        Log_Core::bulid()->append(it);
    }
}

bool OP_Object::faultCode(int id, bool f, uint *cnt, FaultCode code)
{
    uint *dtc = mDev->dtc.code;
    if(f) {
        recoveryLog(id, cnt);
        cnt[id] = 0;
        dtc[id] &= ~code;
    } else {
        cnt[id] += 1;
        dtc[id] |= code;
        mDev->dtc.fault = 16;
    }
    return f;
}

bool OP_Object::volFaultCheck(uchar k, uchar i)
{
    int id = k + i, min = 0;
    uint *src = mOpData->vol;
    uint *cnt = mDev->dtc.cnt[0];
    uint *dest = mDev->output.vol.value;
    int devSpec = mDev->cfg.param.devSpec;
    int isBreaker = mDev->cfg.param.isBreaker;
    if(mOpData->type) dest = mDev->loop.vol.value;
    if(!isBreaker) if((2==devSpec) || (4==devSpec)) min = COM_MIN_VOL;
    bool ret = dataFiltering(dest[id], src[i], COM_MAX_VOL, min);
    if(src[i] == 16*COM_RATE_VOL) {ret = false; /*dest[id] = 0;*/}
    if(!faultCode(id, ret, cnt, FaultCode::DTC_VOL)) {
        faultLog(id, cnt, src[i]);
        if(cm::runTime() > 48*60*60) {
            if(cnt[id] > FAULT_NUM) dest[id] = mDev->line.vol.value[0];
        }
    }
    return ret;
}

bool OP_Object::curFaultCheck(uchar k, uchar i)
{
    int id = k + i;
    uint *src = mOpData->cur;
    uint *cnt = mDev->dtc.cnt[1];
    uint *dest = mDev->output.cur.value;
    if(mOpData->type) dest = mDev->loop.cur.value;
    bool ret = dataFiltering(dest[id], src[i], COM_MAX_CUR);
    if(!faultCode(id, ret, cnt, FaultCode::DTC_CUR)) {
        faultLog(id, cnt, src[i]);
        if(cm::runTime() > 48*60*60) {
            if(cnt[id] > FAULT_NUM) dest[id] = 0;
        }
    }
    return ret;
}

void OP_Object::powFaultCheck(uchar k, uchar i)
{
    int id = k + i;
    uint value = mOpData->pf[i];
    sObjData *obj = &mDev->output;
    if(mOpData->type) obj = &mDev->loop;
    if(value < 100) {
        obj->pf[id] = value;
        obj->artPow[id] = obj->vol.value[id] * obj->cur.value[id] / (COM_RATE_VOL*COM_RATE_CUR);
        obj->pow.value[id] = obj->artPow[id] * obj->pf[id] / COM_RATE_PF;
        obj->reactivePow[id] = obj->artPow[id] - obj->pow.value[id];
    } else {
        cout << value/COM_RATE_PF;
        sEventItem it; it.event_type = tr("PF");
        it.event_content = tr("输出位 %1 功率因素错误％2").arg(id+1).arg(value/COM_RATE_PF);
        Log_Core::bulid()->append(it);
    }
}


void OP_Object::eleFaultCheck(uchar k, uchar i)
{
    int id = k + i;
    bool ret = true;
    uint *src = mOpData->ele;
    uint *cnt = mDev->dtc.cnt[2];
    uint *dest = mDev->output.ele;
    if(mOpData->type) dest = mDev->loop.ele;
    if((dest[id] && src[i]) && (src[i] < 1000)){
        if(src[i] - dest[id] > 2) {
            ret = false;
            faultLog(id, cnt, src[i]);
        } else cnt[id] = 0;
    }

    if(!faultCode(id, ret, cnt, FaultCode::DTC_ELE)) {
        if(cm::runTime() < 48*60*60) dest[id] = src[i];
        else if(cnt[id] > FAULT_NUM) dest[id] = src[i];
    } else dest[id] = src[i];
}

void OP_Object::relayCheck(uint &dst_sw, uint &src_sw)
{
    if(mDev->cfg.param.devSpec < 3) dst_sw = 2;
    else dst_sw = src_sw;

    //if(dst_sw != src_sw) {
    //    if(cnt++) dst_sw = src_sw;
    //} else cnt = 0;
}

void OP_Object::fillData(uchar addr)
{
    sDevData *dev = mDev; uchar k = 0;
    sOpIt *it = mOpData; addr -= 1;
    if(mDev->dtc.fault) mDev->dtc.fault -= 1;
    for(int i=0; i<addr; ++i) k += dev->cfg.nums.boards[i];
    for(int i=0; i<it->size; ++i) {
        volFaultCheck(k, i);
        curFaultCheck(k, i);
        powFaultCheck(k, i);
        eleFaultCheck(k, i);
        //dev->output.relay.sw[k+i] = it->sw[i];
        relayCheck(dev->output.relay.sw[k+i], it->sw[i]);
    }

    dev->offLine = 5;
    dev->rtu.hzs[addr] = it->hz;
    //dev->cfg.nums.boards[addr] = it->size;
    dev->cfg.vers.opVers[addr] = it->version;
    dev->rtu.chipStates[addr] = it->chipStatus;
    for(int i=0; i<8; ++i) dev->rtu.offLines[i] = it->ens[i];
}

void OP_Object::loop_fillData()
{
    sDevData *dev = mDev; uchar k = 0;
    sOpIt *it = mOpData; //mDev->dtc.fault = 0;
    if(mDev->dtc.fault) mDev->dtc.fault -= 1;
    for(int i=0; i<it->size; ++i) {
        if(it->vol[i] < 50*COM_RATE_VOL) it->vol[i] = 0;
        volFaultCheck(k, i);        
        curFaultCheck(k, i);
        powFaultCheck(k, i);
        eleFaultCheck(k, i);                
        //dev->loop.relay.sw[k+i] = it->sw[i];
        //relayCheck(dev->loop.relay.sw[k+i], it->sw[i], m_swCnt[k+i]);
    }

    dev->offLine = 5;
    dev->rtu.hzs[0] = it->hz;
    dev->cfg.vers.opVers[0] = it->version;
}
