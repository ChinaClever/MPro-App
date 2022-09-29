/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_trap.h"
#include "alarm_Updater.h"
#include "agent_core.h"

Agent_Trap::Agent_Trap(QObject *parent)
    : Agent_Set{parent}
{
    QTimer::singleShot(5550,this,SLOT(initTrapSlot()));
}

void Agent_Trap::initTrapSlot()
{
    mModuleOid = mSnmp->moduleOid();
    Alarm_Updater *alarm = Alarm_Updater::bulid();
    //connect(alarm, &Alarm_Updater::alarmSig, this, &Agent_Trap::alarmSlot);

    //timer = new QTimer(this);
    timer->start(3000); ////==========
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
}

void Agent_Trap::timeoutDone()
{
    sDataItem it;
    it.addr = 0;
    it.type = 1;
    it.topic = 2;
    it.subtopic = 3;
    it.id = 0;
    it.value =1;

    alarmSlot(it, 1);
}

void Agent_Trap::alarmSlot(const sDataItem &index, uchar value)
{
    QString msg = "Alarm: ";
    QSNMPOid oid = mModuleOid;

    oid << index.addr;
    msg += " addr " + QString::number(index.addr);

    switch (index.type) {
    case DType::Line: oid << 1;  msg += " line "; break;
    case DType::Loop: oid << 2; msg += " loop "; break;
    case DType::Output: oid << 3; msg += " output "; break;
    case DType::Env: oid << 6; msg += " env "; break;
    case DType::Sensor: oid << 7; msg += " Sensor ";  break;
    default: qDebug() << Q_FUNC_INFO << index.type; break;
    }
    oid << index.id; msg += QString::number(index.id+1);

    switch (index.topic) {
    case DTopic::Relay: oid << 1;  msg += " switch "; break;
    case DTopic::Vol: oid << 2;  msg += " vol "; break;
    case DTopic::Cur: oid << 3;  msg += " cur "; break;
    case DTopic::Pow: oid << 4;  msg += " pow "; break;
    case DTopic::Tem: oid << 6;  msg += " tem "; break;
    case DTopic::Hum: oid << 7;  msg += " hum "; break;
    case DTopic::Door1: oid << 1; msg += " Door1 "; break;
    case DTopic::Door2: oid << 2; msg += " Door2 "; break;
    case DTopic::Water: oid << 3; msg += " Water "; break;
    case DTopic::Smoke: oid << 4; msg += " Smoke "; break;
    default: qDebug() << Q_FUNC_INFO << index.topic; break;
    }

    QSNMPOid dstOid = oid;
    if(index.topic == DTopic::Relay);
    else if(index.topic == DType::Sensor);
    else if(index.type == DType::Env) oid << 7;
    else oid << 8;

    if(value) {
        QString doid = toString(dstOid << 11);
        sAgentCfg *cfg = &Agent_Core::snmpCfg;

        QString ip = cfg->trap1;
        if(ip.size()) sendTrap(ip, doid, toString(oid), msg);

        ip = cfg->trap2;
        if(ip.size()) sendTrap(ip, doid, toString(oid), msg);
        //mSnmp->sendTrap(oid);

        sendTrap("192.168.1.196", doid, toString(oid), msg);
    }
}

//  命令  版本   -c 共同体  管理机  Enterprise-OID  snmp代理地址   陷阱类型 oid   数据类型    数据类型
void Agent_Trap::sendTrap(const QString &ip, const QString &dstOid, const QString &oid, const QString &msg)
{
    QString cmd = "snmptrap -v 2c -c public %1 '' %2 %3 s '%4'";
    QString str = cmd.arg(ip, dstOid, oid, msg);
    system(str.toLatin1().data());
}
