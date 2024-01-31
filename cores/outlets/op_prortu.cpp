/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_prortu.h"
#include "log_core.h"

OP_ProRtu::OP_ProRtu(QObject *parent) : OP_ProCtrl{parent}
{

}

void OP_ProRtu::rtuThrowMessage(const QString &msg)
{
    QString str = "rtu outlet " + msg;
    QString ip = cm::dataPacket()->ota.host;
    if(ip.size()) mNet->writeDatagram(str.toUtf8(), QHostAddress(ip), 21437);
}

bool OP_ProRtu::recvPacket(const QByteArray &array, sOpIt *obj)
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
        obj->temp = *ptr++; obj->type = 0;

        for(int i=1; i<obj->size-1; ++i) {
            obj->vol[i] = getShort(ptr); ptr += 2;
        } for(int i=0; i<obj->size; ++i) obj->tmp_vol[i] = obj->vol[i];

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

void OP_ProRtu::hardwareLog(int addr, const QByteArray &cmd)
{
    if(m_array[addr] != cmd) {
        m_array[addr] = cmd; sEventItem it;
        if(cm::cn()) {it.event_type = tr("执行板通讯");
            it.event_content = tr("执行板无响应 地址:%1 ").arg(addr);
        } else {
            it.event_type = "Executive board communication";
            it.event_content = tr("No response from the execution board addr:%1 ").arg(addr);
        } rtuThrowMessage(it.event_type + it.event_content);
        if(cm::runTime() > 5) Log_Core::bulid()->append(it);
    } //cout << addr << cm::byteArrayToHexStr(cmd);;
}

bool OP_ProRtu::rtuLog(int addr, const QByteArray &array)
{
    bool ret = true;
    static int cnt[DEV_NUM] = {0};
    uchar *ptr = (uchar *)array.data();
    uchar crc = Crc::XorNum(ptr, array.size()-1);
    if(crc != (uchar)array.at(array.size()-1)) {
        ret = false; if(1 == cnt[addr]++) {
            sEventItem it; it.addr = addr;
            if(cm::cn()) {
                it.event_type = tr("执行板通讯");
                it.event_content = tr("收到执行板 %1 异常通讯数据").arg(addr);
            } else {
                it.event_type = "Executive Board";
                it.event_content = tr("Received abnormal communication data from execution board %1").arg(addr);
            } if(cm::runTime() > 5) Log_Core::bulid()->append(it);
        } cout << cm::byteArrayToHexStr(array);
    } else cnt[addr] = 0;
    return ret;
}

bool OP_ProRtu::sendReadCmd(int addr, sOpIt *it)
{
    bool res = false; int k = 6; waitForLock(); QByteArray recv;
    uchar cmd[zCmdLen] = {0x7B, 0xC1, 0x01, 0xA1, 0xB1, 0x01};
    cmd[2] = addr; for(int i=1; i<61; i++) cmd[k++] = 0x00;
    cmd[k++] = 0x44; cmd[k] = Crc::XorNum(cmd,sizeof(cmd)-1);
    int cnt = 1; if(cm::runTime() > 36*60*60) cnt = 3;

    for(int i=0; i<cnt; ++i) {
        recv = transmit(cmd, sizeof(cmd));
        if(recv.size() == zRcvLen) break;
        else cm::mdelay(1200);
    }

    if(recv.size()) res = rtuLog(addr, recv);
    if((recv.size() == zRcvLen) && (recv.at(2) == addr) && res) {
        res = recvPacket(recv, it);
        if(res) m_array[addr].clear();
    } else if(recv.isEmpty()){
        mOpData->size = mDev->cfg.nums.boards[addr-1];
        hardwareLog(addr, QByteArray((char *)cmd, zCmdLen));
    } else {
        cout << addr << recv.size();
        sEventItem it; if(cm::cn()) {
            it.event_type = tr("执行板通讯");
            it.event_content = tr("执行板 %1 数据读取错误: len=%2").arg(addr).arg(recv.size());
        } else {
            it.event_type = "Executive board communication";
            it.event_content = tr("Execution board %1 data read error: len=%2").arg(addr).arg(recv.size());
        } rtuThrowMessage(it.event_type + cm::byteArrayToHexStr(recv));
        mOpData->size = mDev->cfg.nums.boards[addr-1];
        //it.content +=cm::byteArrayToHexStr(recv);
        if(cm::runTime() > 5) Log_Core::bulid()->append(it);
    }

    return res;
}

bool OP_ProRtu::setEndisable(int addr, bool ret, uchar &v)
{
    if(ret) {
        if(v == 1) {
            sEventItem it; it.event_type = tr("Output");
            if(cm::cn()) it.event_content = tr("执行板 %1 连接正常").arg(addr);
            else it.event_content = tr("Execution board %1 is connected normally").arg(addr);
            if(cm::runTime() > 5) Log_Core::bulid()->append(it);
        } v = 5;
    } else if(v > 1){
        if(--v == 1)  {
            sEventItem it; it.event_type = tr("Output");
            if(cm::cn()) it.event_content = tr("执行板 %1 掉线").arg(addr);
            else it.event_content = tr("Execution board %1 dropped").arg(addr);
            if(cm::runTime() > 5) Log_Core::bulid()->append(it);
        }
    }

    if(v < 3) {
        int size = sizeof(mOpData->vol);
        memset(mOpData->activePow, 0, size);
        memset(mOpData->reactivePow, 0, size);
        memset(mOpData->ele, 0, size);
        memset(mOpData->cur, 0, size);
        memset(mOpData->pow, 0, size);
        memset(mOpData->pf, 0, size);
        mOpData->version = 0;
        mDev->dtc.fault = 6;
        mOpData->temp = 0;
        mOpData->size = mDev->cfg.nums.boards[addr-1];
        /*if(cm::runTime() < 74*60*60)*/ memset(mOpData->tmp_vol, 0, size);
        //else if(cm::adcVol() < 8*1000) memset(mOpData->vol, 0, size);
    }

    int t = 0; if(cm::runTime() > 48*60*60) {
        if(!hasCmdWrite()) if(cm::runTime() > 74*60*60) t += 1000;
        if(!hasCmdWrite()) t += QRandomGenerator::global()->bounded(565);
    } cm::mdelay(t + 656);

    return !ret;
}

bool OP_ProRtu::readData(int addr)
{
    if(isOta) {return false;} sn_read(addr);
    bool ret = sendReadCmd(addr, mOpData);  /*发送读命令*/
    setEndisable(addr, ret, mOpData->ens[addr]);
    fillData(addr); //cout << addr << ret;  /*获取设备数据*/
    return ret;
}

