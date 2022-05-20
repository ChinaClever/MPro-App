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

void Agent_Trap::alarmSlot(sAlarmIndex index, uchar value)
{
    QSNMPOid oid = mModuleOid;
    oid << index.addr;

    switch (index.type) {
    case AlarmIndex::Line: oid << 1; break;
    case AlarmIndex::Loop: oid << 2; break;
    case AlarmIndex::Output: oid << 3; break;
    case AlarmIndex::Env: oid << 6; break;
    case AlarmIndex::Sensor: oid << 7; break;
    default: qDebug() << Q_FUNC_INFO << index.type; break;
    } if(index.type != AlarmIndex::Sensor) oid << index.id;

    switch (index.subtopic) {
    case AlarmIndex::Relay: oid << 1; break;
    case AlarmIndex::Vol: oid << 2; break;
    case AlarmIndex::Cur: oid << 3; break;
    case AlarmIndex::Pow: oid << 4; break;
    case AlarmIndex::Tem: oid << 6; break;
    case AlarmIndex::Hum: oid << 7; break;
    case AlarmIndex::Door1: oid << 1; break;
    case AlarmIndex::Door2: oid << 2; break;
    case AlarmIndex::Water: oid << 3; break;
    case AlarmIndex::Smoke: oid << 4; break;
    default: qDebug() << Q_FUNC_INFO << index.subtopic; break;
    }

    if(index.subtopic == AlarmIndex::Relay);
    else if(index.subtopic == AlarmIndex::Sensor);
    else if(index.type == AlarmIndex::Env) oid << 4;
    else oid << 7;

    if(value) mSnmp->sendTrap(oid);
}
