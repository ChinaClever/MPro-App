/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_objctrl.h"

OP_ObjCtrl::OP_ObjCtrl(QObject *parent) : OP_ObjRtu{parent}
{

}

void OP_ObjCtrl::relayCtrl(int id, int on)
{
    if(id) {
        if(sRelay::On == on) openSwitch(id-1); else closeSwitch(id-1);
    } else orderCtrl(on, 1);
    if(sRelay::Reset == on) {
        sRelayUnit *unit = &(mDev->output.relay); mList << id;
        if(id){id--;} int t = unit->powerUpDelay[id]; if(!t) t = 5;
        QTimer::singleShot(t*1000,this,SLOT(relayResetSlot()));
    }
}

void OP_ObjCtrl::relayResetSlot()
{
    if(mList.size()) {
        int id = mList.takeFirst();
        relayCtrl(id, sRelay::On);
    }
}

void OP_ObjCtrl::orderCtrl(int on, uchar all)
{
    if(on) openAllSwitch(all); else closeAllSwitch(all);
}

void OP_ObjCtrl::clearEle(int id)
{
    if(id) setClearEle(id-1); else clearAllEle();
}

void OP_ObjCtrl::setDelay(int id, uchar sec)
{
    if(id) setOutputDelay(id,sec); else setAllDelay(sec);
}

void OP_ObjCtrl::relaysCtrl(int start, int end, int v)
{
    QList<int> ids;
    for(int i=start; i<end; i++) ids.append(i);
    relaysCtrl(ids, v);
}

void OP_ObjCtrl::relaysCtrl(const QList<int> &ids, int v)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++) on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++) off[i] = 0x00;  //关闭有效位
    foreach(auto &i, ids) {
        if(v) setBitControl(i, on);
        else setBitControl(i, off);
    }
    if(ids.size()) funSwitch(on, off);
}

void OP_ObjCtrl::openAllSwitch(uchar all)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0xFF;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位
    funSwitch(on, off, all);
}


void OP_ObjCtrl::setBitControl(int id, uchar *buf)
{
    int index = id/8; int key = id%8;
    buf[index] |= 0x80 >> key;
}

void OP_ObjCtrl::openSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    setBitControl(id, on);
    funSwitch(on, off);
}


void OP_ObjCtrl::openOnlySwitch(int id)
{
    openSwitch(id);
    closeOtherSwitch(id);
}

void OP_ObjCtrl::closeOtherSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    setBitControl(id, off);
    for(int i=0; i<6; i++)  off[i] = ~off[i];
    funSwitch(on, off);
}

void OP_ObjCtrl::closeAllSwitch(uchar all)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0xff;  //关闭有效位
    funSwitch(on, off, all);
}

void OP_ObjCtrl::closeSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位
    setBitControl(id, off);
    funSwitch(on, off);
}

void OP_ObjCtrl::clearAllEle()
{
    uchar cmd[8];
    for(int i=0; i<6; i++) cmd[i] = 0xFF;
    funClearEle(cmd);
}

void OP_ObjCtrl::setClearEle(int id)
{
    uchar cmd[8];
    for(int i=0; i<6; i++) cmd[i] = 0;
    setBitControl(id, cmd);
    funClearEle(cmd);
}

void OP_ObjCtrl::setAllDelay(uchar sec)
{
    sDevData *dev = cm::masterDev();
    uint *cmd = dev->output.relay.powerUpDelay;
    for(int i=0; i<OUTPUT_NUM; i++) cmd[i] = sec;
    funDelay(cmd);
}

void OP_ObjCtrl::setOutputDelay(int id, uchar sec)
{
    sDevData *dev = cm::masterDev();
    uint *cmd = dev->output.relay.powerUpDelay;
    cmd[id-1] = sec;
    funDelay(cmd);
}
