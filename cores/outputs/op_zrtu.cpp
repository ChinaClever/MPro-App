#include "op_zrtu.h"

OP_ZRtu::OP_ZRtu(QObject *parent) : OP_ZCtrl{parent}
{
    mThread = new CThread(this);
    mThread->init(this, SLOT(run()));
}


OP_ZRtu *OP_ZRtu::bulid(QObject *parent)
{
    OP_ZRtu* sington = nullptr;
    if(sington == nullptr) {
        sington = new OP_ZRtu(parent);
    }
    return sington;
}


bool OP_ZRtu::recvPacket(const QByteArray &array, sOpIt *obj)
{
    bool ret = false; int op = 14;
    uchar *ptr = (uchar *)array.data();

    if((*ptr++ == 0x7B) && (*ptr++ == 0xC1))  {
        obj->addr = *ptr++;
        obj->size = *ptr++; obj->hz = *ptr++;
        obj->vol[0] = getShort(ptr); ptr += 2;
        if(obj->size == 4){   //ZPDU四位没有第二个电压
            obj->vol[obj->size-1] = obj->vol[0]; ptr += 2;
        } else {
            obj->vol[obj->size-1] = getShort(ptr); ptr += 2;
        }

        ushort sw = getShort(ptr); ptr += 2; // 开关状态 1表示开，0表示关
        for(int i=0; i<op; ++i)  obj->sw[i] = (sw >> (15-i)) & 1;

        ptr = toShort(ptr, op, obj->cur);
        ptr = toChar(ptr, op, obj->pf);
        ptr = toOutputEle(ptr, op, obj->ele);

        ptr += 3; //忽略三位97
        obj->version = *ptr++;
        obj->chipStatus = *ptr++; // 01表示执行版计量芯片模块损坏，00表示正常。
        ptr++;

        for(int i=1; i<obj->size-1; ++i) {
            obj->vol[i] = getShort(ptr); ptr += 2;
        }

        for(int i=0; i<op; ++i) {
            obj->activePow[i] = obj->vol[i] * obj->cur[i] / 100.0;
            obj->pow[i] = obj->activePow[i] * obj->pf[i] / 100.0;
            obj->reactivePow[i] = obj->activePow[i] - obj->pow[i];
        }
        ret = true;
    } else {
        qDebug() << "Error: OP_ZRtu recvPacket err" << array.toHex();
    }

    return ret;
}

bool OP_ZRtu::sendReadCmd(int addr, sOpIt *it)
{
    bool res = false; int k = 6; waitForLock();
    uchar cmd[zCmdLen] = {0x7B, 0xC1, 0x01, 0xA1, 0xB1, 0x01};
    cmd[2] = addr; for(int i=1; i<61; i++) cmd[k++] = 0x00;
    cmd[k++] = 0x44; cmd[k] = cm::xorNum(cmd,sizeof(cmd)-1);

    QByteArray recv = transmit(cmd, sizeof(cmd));
    if((recv.size() == 127) && (recv.at(2) == addr)) {
        res = recvPacket(recv, it);
    } else {
        qDebug() << "Error: OP_ZRtu send cmd err!" << recv.toHex();
    }

    return res;
}

bool OP_ZRtu::readData(int addr)
{
    if(isOta) return false;
    bool ret = sendReadCmd(addr, mOpData);
    if(ret) fillData(addr);
    return ret;
}

void OP_ZRtu::run()
{
    for(int i=0; mDev->info.opNum; ++i) {
        readData(i+1);
        mThread->msleep(240);
    } mThread->msleep(300);
}