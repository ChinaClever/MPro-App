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
        sNumStrItem item;
        item.addr = it->addr;
        item.id = it->type;
        item.fc = SFnCode::Uuts;
        item.txType = DTxType::TxSnmp; item.rw = 1;
        qstrcpy((char *)item.str, value.toByteArray().data());
        ret = Set_Core::bulid()->setNumStr(item);
    }
    return ret;
}


bool Agent_Set::upAlarmIndex(sDataItem &index)
{
    uchar v=0;
    bool ret = true;
    sIndex *it = &mIndex;
    index.addr = it->addr;
    index.id = it->id;
    index.rw = 1;

    switch (it->fc) {
    case 1: v = DType::Line; break;
    case 2: v = DType::Loop; break;
    case 3: v = DType::Output; break;
    case 6: v = DType::Env; break;
    default: ret = false; break;
    } index.type = v;

    switch (it->type) {
    case 2: v = DTopic::Vol; break;
    case 3: v = DTopic::Cur; break;
    case 4: v = DTopic::Pow; break;
    case 5: v = DTopic::Ele; break;
    case 6: v = DTopic::Tem; break;
    case 7: v = DTopic::Hum; break;
    default: ret = false; break;
    } index.topic = v;

    if(index.type < 6) {
        switch (it->subtopic) {
        case 2: v = DSub::Rated; break;
        case 3: v = DSub::VMax; break;
        case 4: v = DSub::VCrMax; break;
        case 5: v = DSub::VCrMin; break;
        case 6: v = DSub::VMin; break;
        case 7: v = DSub::EnAlarm; break;
        default: ret = false; break;
        }
    } else {
        switch (it->subtopic) {
        case 2: v = DSub::VMax; break;
        case 3: v = DSub::VMin; break;
        case 4: v = DSub::EnAlarm; break;
        default: ret = false; break;
        }
    }
    index.subtopic = v;
    index.txType = DTxType::TxSnmp;

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

bool Agent_Set::relayCtrl(const QVariant &value)
{
    sDataItem unit;
    unit.rw = 1;
    unit.id = mIndex.id;
    unit.addr = mIndex.addr;
    unit.type = DType::Output;
    unit.topic = DTopic::Relay;
    unit.subtopic = DSub::Value;
    unit.txType = DTxType::TxSnmp;
    unit.value = value.toUInt();
    return Set_Core::bulid()->setting(unit);
}

bool Agent_Set::setOutputName(const QVariant &value)
{
    QString name = value.toString();
    sNumStrItem item; item.txType = DTxType::TxSnmp;
    item.fc = SFnCode::OutputName; item.rw = 1;
    item.addr = mIndex.addr; item.id = mIndex.id;
    qstrcpy((char *)item.str, name.toLatin1().data());
    return Set_Core::bulid()->setNumStr(item);
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
