/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_artu.h"
#include "log_core.h"

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
        ptr = toOutputEle(ptr, op, it->ele);
        ushort sw = *ptr++; // 开关状态 1表示开，0表示关
        for(int i=0; i<op; ++i)  it->sw[i] = (sw >> (op-i)) & 1;
        it->version = *ptr++; it->type = 1;
        //cout << sw << *ptr++ << *ptr++;

        for(int i=0; i<op; ++i) {
            it->activePow[i] = it->vol[i] * it->cur[i] / 100.0;
            if(it->pow[i]) it->pf[i] = it->activePow[i] * 100.0 / it->pow[i]; else it->pf[i] = 0;
            it->reactivePow[i] = it->activePow[i] - it->pow[i];
        }

    } else ret = false;

    return ret;
}


bool OP_ARtu::loop_setEndisable(bool ret, uchar &v)
{
    if(ret) {
        if(v == 1) {
            sEventItem it; it.event_type = tr("Metering plate");
            if(cm::cn()) it.event_content = tr("计量板连接正常");
            else it.event_content = "The connection of the metering board is normal";
            Log_Core::bulid()->append(it);
        } v = 5;
    } else if(v > 1){
        if(--v == 1)  {
            sEventItem it; it.event_type = tr("Metering plate");
            if(cm::cn()) it.event_content = tr("总计量板异常");
            else it.event_content = "Abnormal total metering board";
            Log_Core::bulid()->append(it);

            int size = sizeof(mOpData->vol);
            memset(mOpData->vol, 0, size);
            memset(mOpData->cur, 0, size);
            memset(mOpData->pf, 0, size);
        }
    }

    int t = 0; if(cm::runTime() > 48*60*60) {
        t = QRandomGenerator::global()->bounded(565);
        if(cm::runTime() > 74*60*60) t += 1000;
    } cm::mdelay(t + 360);

    return !ret;
}

bool OP_ARtu::loop_readData()
{
    bool res = false; int k = 5; waitForLock(); uchar addr = 4;
    uchar cmd[zCmdLen] = {0x7B, 0xC1, 0x01, 0xA9, 0xB9, 0x01};
    cmd[2] = addr; for(int i=0; i<61; i++) cmd[k++] = 0x00;
    cmd[k++] = 0x44; cmd[k] = Crc::XorNum(cmd,sizeof(cmd)-1);
    QByteArray recv = transmit(cmd, sizeof(cmd)); //cout << recv.size();
    if((recv.size() == 62) && (recv.at(2) == addr)) {
        res = loop_recvPacket(recv, mOpData);
        if(res) loop_fillData();
    } else if(recv.size()) cout << recv.size();

    return loop_setEndisable(res, mOpData->ens[0]);
}
