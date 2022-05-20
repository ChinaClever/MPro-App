/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_set.h"
#include "set_core.h"

Agent_Set::Agent_Set(QObject *parent)
    : Agent_Get{parent}
{
    QTimer::singleShot(3550,this,SLOT(initSetSlot()));
}

void Agent_Set::initSetSlot()
{
    connect(mSnmp, &SnmpModule::snmpSetSig, this, &Agent_Set::snmpSetSlot);
}

bool Agent_Set::toIndex(uint addr, const QSNMPOid &oid)
{
    bool ret = false; int k = 6; sIndex *it = &mIndex;
    if((oid.at(k++) == 30966) && (oid.at(k++) == 11)) {
        if(oid.at(k++) == addr) {
            it->addr = addr;
            it->fc = oid.at(k++);
            it->id = oid.at(k++);
            it->type = oid.at(k++);
            it->subtopic = oid.at(k++);
            ret = true;
        }
    }

    return ret;
}

bool Agent_Set::uutSet(const QVariant &value)
{
    bool ret = false;
    sIndex *it = &mIndex;
    if(it->id == 0) {
        sDevData *dev = cm::devData(it->addr);
        sUutInfo uut = dev->uut; char *ptr = nullptr;
        switch (it->type) {
        case 1: ptr = uut.idc; break;
        case 2: ptr = uut.room; break;
        case 3: ptr = uut.module; break;
        case 4: ptr = uut.cab; break;
        case 5: ptr = uut.road; break;
        case 6: ptr = uut.devName; break;
        }
        if(ptr) {
            qstrcpy((char *)ptr, value.toByteArray().data());
            ret = Set_Core::bulid()->setUut(it->addr, uut);
        }
    }
    return ret;
}

bool Agent_Set::setAlarmUnit(sSetAlarmUnit &unit, const QVariant &value)
{
    bool ret = true;
    sIndex *it = &mIndex;
    uint v = value.toUInt();
    switch (it->subtopic) {
    case 2: unit.rated = v; break;
    case 3: unit.max = v; break;
    case 4: unit.crMax = v; break;
    case 5: unit.crMin = v; break;
    case 6: unit.min = v; break;
    default: ret = false; break;
    }

    if(ret) ret = Set_Core::bulid()->setAlarm(unit);
    return ret;
}

bool Agent_Set::upAlarmIndex(sDIndex &index)
{
    bool ret = true;
    sIndex *it = &mIndex;
    index.addr = it->addr;
    index.id = it->id;

    switch (it->fc) {
    case 1: index.type = DType::Line; break;
    case 2: index.type = DType::Loop; break;
    case 3: index.type = DType::Output; break;
    case 6: index.type = DType::Env; break;
    default: ret = false; break;
    }

    switch (it->type) {
    case 2: index.topic = DTopic::Vol; break;
    case 3: index.topic = DTopic::Cur; break;
    case 4: index.topic = DTopic::Pow; break;
    case 6: index.topic = DTopic::Tem; break;
    case 7: index.topic = DTopic::Hum; break;
    default: ret = false; break;
    }

    return ret;
}

bool Agent_Set::upAlarmData(sSetAlarmUnit &unit)
{
    int id = unit.index.id; bool ret = true;
    sAlarmUnit *it = Set_Core::bulid()->getAlarmUnit(unit.index);
    if(it) {
        unit.max = it->max[id];
        unit.crMax = it->crMax[id];
        unit.crMin = it->crMin[id];
        unit.min = it->min[id];
    } else ret = false;
    return ret;
}

bool Agent_Set::setAlarm(const QVariant &value)
{
    sSetAlarmUnit unit;
    bool ret = upAlarmIndex(unit.index);
    if(ret) ret = upAlarmData(unit);
    if(ret) ret = setAlarmUnit(unit, value);
    return ret;
}

bool Agent_Set::relayCtrl(const QVariant &value)
{
    uint v = value.toUInt();
    uchar addr = mIndex.addr; uchar id = mIndex.id;
    return Set_Core::bulid()->outputRelayCtrl(addr, id, v);
}

bool Agent_Set::setOutputName(const QVariant &value)
{
    QString name = value.toString();
    uchar addr = mIndex.addr; uchar id = mIndex.id;
    return Set_Core::bulid()->outputNameSet(addr, id, name);
}

void Agent_Set::snmpSetSlot(uint addr, const QSNMPOid &oid, const QVariant &value)
{
    sIndex *it = &mIndex;
    bool ret = toIndex(addr, oid);
    if(ret) {
        if(0 == it->fc) ret = uutSet(value);
        else if((3 == it->fc) && (0 == it->type)) ret = setOutputName(value);
        else if((3 == it->fc) && (1 == it->type)) ret = relayCtrl(value);
        else ret = setAlarm(value);
    }

    if(!ret) qDebug() << Q_FUNC_INFO << addr << oid << value;
}
