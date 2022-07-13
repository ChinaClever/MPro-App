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
        } else qCritical() << "Error: Cascade Master fc" << it.fc;
    }
    qDebug() << Q_FUNC_INFO << addr << ret;

    return ret;
}


void Cascade_Master::masterReadDevs()
{
    using namespace cm;
    uint size = masterDev()->info.slaveNum;
    for(uint i=0; i<size; ++i) {
        bool ret = masterRead(i+1);
        setEndisable(i, ret, devData(i+1)->offLine);
    } mdelay(320);
}

void Cascade_Master::setEndisable(int addr, bool ret, uchar &v)
{
    if(ret) {
        if(v == 0) {
            //qDebug() << " error slave up";
            sSysItem it; it.module = tr("级联");
            it.content = tr("副机 %1 连接正常").arg(addr+1);
            Log_Core::bulid(this)->append(it);
        } v = 3;
    } else if(v > 0){
        if(--v == 0)  {
            //qDebug() << " error slave lose";
            sSysItem it; it.module = tr("级联");
            it.content = tr("副机 %1 掉线").arg(addr+1);
            Log_Core::bulid(this)->append(it);
        }
    } cm::mdelay(100);
}

bool Cascade_Master::masterSeting(const sDataItem &unit)
{
    uchar fc = fc_setting; if(isOta) return isOta;
    QByteArray array = cm::toByteArray(unit);
    return writeData(fc, unit.addr, array);
}


bool Cascade_Master::masterSetNumStr(const sNumStrItem &it)
{
    uchar fc = fc_setNumStr; if(isOta) return isOta;
    QByteArray array = cm::toByteArray(it);
    return writeData(fc, it.addr, array);
}

