/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_artu.h"

OP_ARtu::OP_ARtu(QObject *parent)
    : OP_ObjCtrl{parent}
{

}

bool OP_ARtu::loop_recvPacket(const QByteArray &array, sOpIt *it)
{
    bool ret = true; int op = 6;
    uchar *ptr = (uchar *)array.data();
    if((*ptr++ == 0x7B) && (*ptr++ == 0xC1))  {
        it->addr = *ptr++;
        it->hz = *ptr++;
        it->size = op;

        ptr = toShort(ptr, op, it->vol);
        ptr = toShort(ptr, op, it->cur);
        ptr = toShort(ptr, op, it->pow);
        ushort sw = *ptr++; // 开关状态 1表示开，0表示关
        for(int i=0; i<op; ++i)  it->sw[i] = (sw >> (7-i)) & 1;
        it->version = *ptr++; it->type = 1;

        for(int i=0; i<op; ++i) {
            it->activePow[i] = it->vol[i] * it->cur[i] / 100.0;
            if(it->pow[i]) it->pf[i] = it->activePow[i] * 100.0 / it->pow[i]; else it->pf[i] = 0;
            it->reactivePow[i] = it->activePow[i] - it->pow[i];
        }

    } else ret = false;

    return ret;
}

bool OP_ARtu::loop_readData()
{
    bool res = false; int k = 5; waitForLock(); uchar addr = 4;
    uchar cmd[zCmdLen] = {0x7B, 0xC1, 0x01, 0xA9, 0xB9, 0x01};
    cmd[2] = addr; for(int i=0; i<61; i++) cmd[k++] = 0x00;
    cmd[k++] = 0x44; cmd[k] = Crc::XorNum(cmd,sizeof(cmd)-1);

    QByteArray recv = transmit(cmd, sizeof(cmd));
    if((recv.size() == 61) && (recv.at(2) == addr)) {
        res = loop_recvPacket(recv, mOpData);
        if(res) loop_fillData();
    }

    int t = 0; if(cm::runTime() > 48*60*60) {
        t = QRandomGenerator::global()->bounded(565);
    } cm::mdelay(t + 360);

    return res;
}
