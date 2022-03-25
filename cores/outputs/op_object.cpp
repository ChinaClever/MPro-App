#include "op_object.h"

OP_Object::OP_Object(QObject *parent) : SerialPort{parent}
{
    mOpData = new sOpIt;
    memset(mOpData, 0, sizeof(sOpIt));
    mDev = cm::masterDev();
}


void OP_Object::fillData(uchar addr)
{
    sDevData *dev = mDev;
    sOpIt *it = mOpData; uchar k = 0;
    dev->info.ops[addr] = it->size; addr -= 1;
    for(int i=0; i<addr; ++i) k += dev->info.ops[i];

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

    dev->info.hzs[addr] = it->hz;
    dev->info.ops[addr] = it->size;
    dev->info.opVers[addr] = it->version;
    dev->info.chipStates[addr] = it->chipStatus;
}
