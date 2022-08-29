/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_object.h"
#include "log_core.h"

OP_Object::OP_Object(QObject *parent) : SerialPort{parent}
{
    mOpData = new sOpIt;
    memset(mOpData, 0, sizeof(sOpIt));
    mDev = cm::masterDev();
}

bool OP_Object::dataFiltering(uint &dest, uint &src, uint max, uint min)
{


}

void OP_Object::fillData(uchar addr)
{
    sDevData *dev = mDev; uint value;
    sOpIt *it = mOpData; uchar k = 0;
    dev->cfg.nums.boards[addr] = it->size; addr -= 1;
    for(int i=0; i<addr; ++i) k += dev->cfg.nums.boards[i];

    for(int i=0; i<it->size; ++i) {        
        dev->output.vol.value[k+i] = it->vol[i];
        dev->output.cur.value[k+i] = it->cur[i];
        dev->output.pow.value[k+i] = it->pow[i];
        dev->output.artPow[k+i] = it->activePow[i];
        dev->output.reactivePow[k+i] = it->reactivePow[i];

        dev->output.pf[k+i] = it->pf[i];
        dev->output.ele[k+i] = it->ele[i];
        dev->output.relay.sw[k+i] = it->sw[i];
    }

    dev->offLine = 3;
    dev->rtu.hzs[addr] = it->hz;
    dev->cfg.nums.boards[addr] = it->size;
    dev->cfg.vers.opVers[addr] = it->version;
    dev->rtu.chipStates[addr] = it->chipStatus;
    for(int i=0; i<4; ++i) dev->rtu.offLines[i] = it->ens[i];
}

bool OP_Object::appendLog(int addr)
{
    sSysItem it; it.module = tr("Output");
    it.content = tr("执行板 %1 连接正常").arg(addr+1);
    Log_Core::bulid(this)->append(it);
}
