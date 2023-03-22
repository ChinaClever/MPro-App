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
    if((snmpCfg.enV3) || (snmpCfg.enV2)) {
        QTimer::singleShot(5550,this,&Agent_Set::initSetSlot);
    }
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
            it->rw = oid.at(k++);
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
        sCfgItem item;
        item.addr = it->addr;
        item.fc = it->type;
        item.type = SFnCode::Uuts;
        item.txType = DTxType::TxSnmp;
        ret = Set_Core::bulid()->setCfg(item, value);
    }
    return ret;
}

bool Agent_Set::paramSet(const QVariant &value)
{
    bool ret = false;
    sIndex *it = &mIndex;
    if(it->id == 2) {
        sCfgItem item;
        item.addr = it->addr;
        item.fc = it->type;
        item.type = SFnCode::EDevInfo;
        item.txType = DTxType::TxSnmp;
        ret = Set_Core::bulid()->setCfg(item, value);
    }
    return ret;
}

bool Agent_Set::cfgSet(const QVariant &value)
{
    bool ret = false;
    sIndex *it = &mIndex;
    switch (it->id) {
    case 0: ret = uutSet(value); break;
    case 2: ret = paramSet(value); break;
    default: cout << it->id << value; break;
    }

    return ret;
}

bool Agent_Set::upAlarmIndex(sDataItem &index)
{
    sIndex *it = &mIndex;
    uchar v=0; bool ret = true;
    index.txType = DTxType::TxSnmp;
    index.addr = it->addr;
    index.id = it->id;
    index.rw = 1;

    switch (it->fc) {
    case 1: v = DType::Line; break;
    case 2: v = DType::Loop; break;
    case 3: v = DType::Output; it->type +=1; break;
    case 4: v = DType::Group; it->type +=2; break;
    case 5: v = DType::Dual; it->type +=2; break;
    case 6: v = DType::Env; it->type +=5; break;
    default: ret = false; break;
    } index.type = v;

    switch (it->type) {
    case 1: v = DTopic::Vol; break;
    case 2: v = DTopic::Cur; break;
    case 3: v = DTopic::Pow; break;
    case 4: v = DTopic::Ele; break;
    case 6: v = DTopic::Tem; break;
    case 7: v = DTopic::Hum; break;
    default: ret = false; break;
    } index.topic = v;

    switch (it->subtopic) {
    case 1: v = DSub::EnAlarm; break;
    case 2: v = DSub::VMax; break;
    case 3: v = DSub::VCrMax; break;
    case 4: v = DSub::VCrMin; break;
    case 5: v = DSub::VMin; break;
    case 6: v = DSub::Rated; break;
    case 7: v = DSub::Alarm; break;
    default: ret = false; break;
    } index.subtopic = v;

    return ret;
}


bool Agent_Set::setAlarm(const QVariant &value)
{
    sDataItem unit;
    bool ret = upAlarmIndex(unit);
    if(ret) unit.value = value.toInt();
    if(ret) ret = Set_Core::bulid()->setting(unit);
    return ret;
}

bool Agent_Set::relayGroupCtrl(const QVariant &value)
{
    sDataItem unit;
    unit.rw = 1;
    unit.id = mIndex.id;
    unit.addr = mIndex.addr;
    unit.type = DType::Group;
    unit.topic = DTopic::Relay;
    unit.subtopic = DSub::Value;
    unit.txType = DTxType::TxSnmp;
    unit.value = value.toUInt();
    return Set_Core::bulid()->setting(unit);
}

bool Agent_Set::relayDualCtrl(const QVariant &value)
{
    sDataItem unit;
    unit.rw = 1;
    unit.id = mIndex.id;
    unit.addr = mIndex.addr;
    unit.type = DType::Dual;
    unit.topic = DTopic::Relay;
    unit.subtopic = DSub::Value;
    unit.txType = DTxType::TxSnmp;
    unit.value = value.toUInt();
    return Set_Core::bulid()->setting(unit);
}

bool Agent_Set::setOutputName(const QVariant &value)
{
    sCfgItem item;
    item.txType = DTxType::TxSnmp;
    item.type = SFnCode::OutputName;
    item.addr = mIndex.addr; item.fc = mIndex.id;
    return Set_Core::bulid()->setCfg(item, value);
}

bool Agent_Set::setName(int type,const QVariant &value)
{
    sCfgItem item;
    item.txType = DTxType::TxSnmp;
    item.type = 19 + type;
    item.addr = mIndex.addr; item.fc = mIndex.id;
    return Set_Core::bulid()->setCfg(item, value);
}

bool Agent_Set::ctrlOutput(const QVariant &value)
{
    sDataItem unit;
    unit.rw = 1;
    unit.id = mIndex.id;
    unit.addr = mIndex.addr;
    unit.type = DType::Output;
    unit.txType = DTxType::TxSnmp;
    uchar v=1; sIndex *it = &mIndex;

    if(2 == it->type) {
        unit.topic = DTopic::Ele;
    } else {
        switch (it->subtopic) {
        case 1: v = DSub::Value; break;
        case 2: v = DSub::RelayEn; break;
        case 3: v = DSub::Alarm; break;
        case 4: v = DSub::Rated; break;
        case 5: v = DSub::UpDelay; break;
        case 6: v = DSub::ResetDelay; break;
        case 7: v = DSub::OverrunOff; break;
        default: cout << it->subtopic; break;
        }  unit.topic = DTopic::Relay;
    }

    unit.subtopic = v;
    unit.value = value.toUInt();
    return Set_Core::bulid()->setting(unit);
}

void Agent_Set::snmpSetSlot(uint addr, const QSNMPOid &oid, const QVariant &value)
{
    sIndex *it = &mIndex; m_oid = oid;
    bool ret = toIndex(addr, oid);
    if(ret) {
        if(2 == it->rw) {
            if(0 == it->fc) ret = cfgSet(value);
            else if(0 == it->type) ret = setName(it->fc, value);
            else ret = setAlarm(value);
        } else if(3 == it->rw) {
            switch (it->fc) {
            case 1: ret = ctrlOutput(value); break;
            case 2: ret = relayGroupCtrl(value); break;
            case 3: ret = relayDualCtrl(value); break;
            }
        }
    }

    if(!ret) cout << addr << oid << value;
}
