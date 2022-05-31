/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_trap.h"
#include "alarm_Updater.h"

Agent_Trap::Agent_Trap(QObject *parent)
    : Agent_Set{parent}
{
    QTimer::singleShot(5550,this,SLOT(initTrapSlot()));
}

void Agent_Trap::initTrapSlot()
{
    mModuleOid = mSnmp->moduleOid();
    Alarm_Updater *alarm = Alarm_Updater::bulid();
    connect(alarm, &Alarm_Updater::alarmSig, this, &Agent_Trap::alarmSlot);
}

void Agent_Trap::alarmSlot(const sDataItem &index, uchar value)
{
    QSNMPOid oid = mModuleOid;
    oid << index.addr;

    switch (index.type) {
    case DType::Line: oid << 1; break;
    case DType::Loop: oid << 2; break;
    case DType::Output: oid << 3; break;
    case DType::Env: oid << 6; break;
    case DType::Sensor: oid << 7; break;
    default: qDebug() << Q_FUNC_INFO << index.type; break;
    }

    switch (index.topic) {
    case DTopic::Relay: oid << 1; break;
    case DTopic::Vol: oid << 2; break;
    case DTopic::Cur: oid << 3; break;
    case DTopic::Pow: oid << 4; break;
    case DTopic::Tem: oid << 6; break;
    case DTopic::Hum: oid << 7; break;
    case DTopic::Door1: oid << 1; break;
    case DTopic::Door2: oid << 2; break;
    case DTopic::Water: oid << 3; break;
    case DTopic::Smoke: oid << 4; break;
    default: qDebug() << Q_FUNC_INFO << index.topic; break;
    }

    if(index.topic == DTopic::Relay);
    else if(index.topic == DType::Sensor);
    else if(index.type == DType::Env) oid << 4;
    else oid << 7;

    if(value) mSnmp->sendTrap(oid);
}
