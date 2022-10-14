/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_zrtu.h"
#include "log_core.h"

OP_ZRtu::OP_ZRtu(QObject *parent) : OP_ZCtrl{parent}
{

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

        //for(int i=0; i<op; ++i) {
        //    obj->activePow[i] = obj->vol[i] * obj->cur[i] / 100.0;
        //    obj->pow[i] = obj->activePow[i] * obj->pf[i] / 100.0;
        //    obj->reactivePow[i] = obj->activePow[i] - obj->pow[i];
        //}

        ret = true;
    } else {
        cout << array.toHex();
    }

    return ret;
}

void OP_ZRtu::hardwareLog(int addr, const QByteArray &cmd)
{
    if(m_array[addr] != cmd) {
        m_array[addr] = cmd; sHardwareItem it; it.module = tr("执行板通讯");
        it.content = tr("执行板无响应 addr:%1 ").arg(addr+1);
        Log_Core::bulid(this)->append(it);
    }
}

bool OP_ZRtu::sendReadCmd(int addr, sOpIt *it)
{
    bool res = false; int k = 6; waitForLock();
    uchar cmd[zCmdLen] = {0x7B, 0xC1, 0x01, 0xA1, 0xB1, 0x01};
    cmd[2] = addr; for(int i=1; i<61; i++) cmd[k++] = 0x00;
    cmd[k++] = 0x44; cmd[k] = Crc::XorNum(cmd,sizeof(cmd)-1);

    QByteArray recv = transmit(cmd, sizeof(cmd));
    if((recv.size() == zRcvLen) && (recv.at(2) == addr)) {
        res = recvPacket(recv, it);
        if(res) m_array[addr].clear();
    } else if(recv.isEmpty()){
        cout << addr;
        hardwareLog(addr, QByteArray((char *)cmd, zCmdLen));
    } else {
        cout << addr << recv.size(); //
        sSysItem it; it.module = tr("执行板通讯");
        it.content = tr("执行板 %1 数据读取错误: len=%2").arg(addr).arg(recv.size());
        //it.content +=cm::byteArrayToHexStr(recv);
        Log_Core::bulid(this)->append(it);
    }

    return res;
}

bool OP_ZRtu::setEndisable(int addr, bool ret, uchar &v)
{
    if(ret) {
        if(v == 1) {
            sSysItem it; it.module = tr("Output");
            it.content = tr("执行板 %1 连接正常").arg(addr);
            Log_Core::bulid(this)->append(it);
        } v = 5;
    } else if(v > 1){
        if(--v == 1)  {
            sSysItem it; it.module = tr("Output");
            it.content = tr("执行板 %1 掉线").arg(addr);
            Log_Core::bulid(this)->append(it);

            int size = sizeof(mOpData->vol);
            memset(mOpData->vol, 0, size);
            memset(mOpData->cur, 0, size);
            memset(mOpData->pf, 0, size);
        }
    }

    int t = 0; if(cm::runTime() > 48*60*60) {
        t = QRandomGenerator::global()->bounded(565);
    } cm::mdelay(t + 360);

    return ret;
}

bool OP_ZRtu::readData(int addr)
{
    if(isOta) return false;
    bool ret = sendReadCmd(addr, mOpData); fillData(addr); //cout << addr << ret;
    return setEndisable(addr, ret, mOpData->ens[addr]);
}

void OP_ZRtu::run()
{
    while (isRun) {
        int size = mDev->cfg.nums.boardNum;
        for(int i=0; i<size; ++i) {
            cmsWriteSlot(175);
            ota_updates();
            readData(i+1);
        } cm::mdelay(10);
    }
}
