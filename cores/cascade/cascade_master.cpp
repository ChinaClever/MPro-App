/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_master.h"
#include "log_core.h"

Cascade_Master::Cascade_Master(QObject *parent) : Cascade_Unserialize{parent}
{

}

bool Cascade_Master::masterRead(uchar addr)
{
    bool ret = false; if(isOta) return isOta;
    QVector<c_sFrame> its = readData(fc_readDev, addr); /*读取设备地址对应的数据*/
    for(auto &it: its) {
        if((it.fc == fc_readDev) && it.srcAddr){
            deDataStream(it.data); ret = unSequence(it.srcAddr);
        } else cout << "Error: Cascade Master fc" << it.fc << it.srcAddr;
    } if(ret) cmsWrite(155); else cout << addr << ret;

    return ret;
}


void Cascade_Master::masterReadDevs()
{
    using namespace cm; int t = 100; bool ret;
    uint size = masterDev()->cfg.nums.slaveNum; /*获取副机的数量*/
    int cnt = 1; if(cm::runTime() > 24*60*60) cnt = 3;  /*最近开关运行时间超过1小时*/
    for(uint i=1; i<=size; ++i) {
        for (int k = 0; k < cnt; ++k) {
            ret = masterRead(i);
            if(ret) break; else mdelay(1500);
        } setEndisable(i, ret, devData(i)->offLine);    /*传入的参数更新设备的状态*/
        if(hasCmdWrite()) return;
    }

    if(runTime() > 48*60*60) if(!hasCmdWrite()) t += 500;   /*如果运行时间超过48小时且没有写入的命令，将t增加500*/
    if(!hasCmdWrite()) if(runTime() > 74*60*60) t += 1000;  /*如果没有写入的命令且运行时间超过74小时，将t增加1000*/
    t = QRandomGenerator::global()->bounded(t); mdelay(550+t);  /*随机生成0到t之间的随机数，延时t+550毫秒*/
}

void Cascade_Master::setEndisable(int addr, bool ret, uchar &v)
{
    if(ret) {
        if(v == 1) {
            sEventItem it; it.addr = addr;
            if(cm::cn()) {
                it.event_type = tr("级联");
                it.event_content = tr("副机 %1 连接正常").arg(addr);
            } else {
                it.event_type = "cascade";
                it.event_content = tr("Connection from machine %1 is normal").arg(addr);
            } Log_Core::bulid()->append(it);
        } if(cm::runTime() > 48*60*60) v = 6; else v = 3;
        if(cm::runTime() > 74*60*60) v = 10;
    } else if(v > 1){
        if(--v == 1)  {
            sEventItem it; it.addr = addr;
            if(cm::cn()) {
                it.event_type = tr("级联");
                it.event_content = tr("副机 %1 掉线").arg(addr);
            } else {
                it.event_type = "cascade";
                it.event_content = tr("slave %1 dropped").arg(addr);
            } Log_Core::bulid()->append(it);
            cout << addr << ret << v;
        }
    } cm::mdelay(355);
}

bool Cascade_Master::masterSeting(const sDataItem &unit)
{
    uchar fc = fc_setting; if(isOta || getAddress()) return isOta;  /*是否处于OTA模式*/
    QByteArray array = cm::toByteArray(unit);
    return writeData(fc, unit.addr, array); /*调用函数将数据写入到设备中，并将写入操作的结果作为函数的返回值*/
}


bool Cascade_Master::masterSetCfg(const sCfgItem &it, const QVariant &v)
{
    uchar fc = fc_setCfg; if(isOta || getAddress()) return isOta;   /*是否处于OTA模式*/
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << cm::toByteArray(it) << v;
    return writeData(fc, it.addr, array);
}

