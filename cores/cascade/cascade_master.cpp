#include "cascade_master.h"

Cascade_Master::Cascade_Master(QObject *parent) : Cascade_Unserialize{parent}
{

}

bool Cascade_Master::masterRead(uchar addr)
{
    bool ret = false; if(isOta) return isOta;
    QByteArray rcv = readData(fc_readDev, addr, sizeof(c_sDevData));
    if((rcv.size()>4)) {
        deDataStream(rcv);
        ret = unSequence(addr);
    }

    return ret;
}

void Cascade_Master::masterReadDevs()
{
    using namespace cm;
    sDevData *dev = masterDev();
    for(int i=0; i<dev->info.slaveNum; ++i) {
        bool ret = masterRead(i+1);
        if(!ret){} ////========
        mdelay(120);
    } mdelay(320);
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

bool Cascade_Master::masterRelaySet(int addr, int id, uchar type, uchar delay)
{
    uchar fc = fc_relaySet; if(isOta) return isOta;
    QByteArray array;  array.append(id); array.append(type); array.append(delay);
    return writeData(fc, addr, array);
}