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
    QVector<c_sFrame> its = readData(fc_readDev, addr);
    for(auto &it: its) {
        if((it.fc == fc_readDev) && it.srcAddr){
            deDataStream(it.data); ret = unSequence(it.srcAddr);
        } else cout << "Error: Cascade Master fc" << it.fc << it.srcAddr;
    } if(ret) cmsWrite(155); else cout << addr << ret;

    return ret;
}


void Cascade_Master::masterReadDevs()
{
    using namespace cm; int t = 100;
    uint size = masterDev()->cfg.nums.slaveNum;
    for(uint i=1; i<=size; ++i) {
        bool ret = masterRead(i); // cout << ret << i;
        if(!ret) {mdelay(1000); ret = masterRead(i);}
        setEndisable(i, ret, devData(i)->offLine);
        if(hasCmdWrite()) return;
    }

    if(runTime() > 48*60*60) if(!hasCmdWrite()) t += 500;
    if(!hasCmdWrite()) if(runTime() > 74*60*60) t += 1000;
    t = QRandomGenerator::global()->bounded(t); mdelay(450+t);
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
    uchar fc = fc_setting; if(isOta || getAddress()) return isOta;
    QByteArray array = cm::toByteArray(unit);
    return writeData(fc, unit.addr, array);
}


bool Cascade_Master::masterSetCfg(const sCfgItem &it, const QVariant &v)
{
    uchar fc = fc_setCfg; if(isOta || getAddress()) return isOta;
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << cm::toByteArray(it) << v;
    return writeData(fc, it.addr, array);
}

