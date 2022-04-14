#include "cascade_master.h"

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

    return ret;
}


void Cascade_Master::masterReadDevs()
{
    using namespace cm;
    uint size = masterDev()->info.slaveNum;
    for(uint i=0; i<size; ++i) {
        bool ret = masterRead(i+1);
        setEndisable(ret, devData(i+1)->offLine);
    } mdelay(320);
}

void Cascade_Master::setEndisable(bool ret, uchar &v)
{
    if(ret) v = 3;
    else if(v > 0){
        if(--v == 0)  qDebug() << " error slave lose";
    } cm::mdelay(100);
}

bool Cascade_Master::masterSetAlarm(const sSetAlarmUnit &unit)
{
    uchar fc = fc_writeAlarm; if(isOta) return isOta;
    QByteArray array = cm::toByteArray(unit);
    return writeData(fc, unit.index.addr, array);
}

bool Cascade_Master::masterRelayCtrl(uchar addr, uchar id, uchar on)
{
    uchar fc = fc_relayCtrl; if(isOta) return isOta;
    QByteArray array;  array.append(id); array.append(on);
    return writeData(fc, addr, array);
}

bool Cascade_Master::masterDelaySet(int addr, int id, uchar type, uchar delay)
{
    uchar fc = fc_delaySet; if(isOta) return isOta;
    QByteArray array;  array.append(id); array.append(type); array.append(delay);
    return writeData(fc, addr, array);
}
