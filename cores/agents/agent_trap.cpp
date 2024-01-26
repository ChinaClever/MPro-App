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
    if((snmpCfg.enV3) || (snmpCfg.enV2)) {
        QTimer::singleShot(6550,this,&Agent_Trap::initTrapSlot);
    }
}

void Agent_Trap::initTrapSlot()
{
    mModuleOid = mSnmp->moduleOid();
    Alarm_Updater *alarm = Alarm_Updater::bulid();
    connect(alarm, &Alarm_Updater::alarmSig, this, &Agent_Trap::alarmSlot);

    //timer = new QTimer(this);
    //timer->start(3000); ////==========
    //connect(timer, &QTimer::timeout, this, &Agent_Trap::timeoutDone);
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
    QSNMPOid oid = mModuleOid; QString msg = "Alarm ";
    if(value) msg += "elimination "; else msg += "generation ";
    oid << index.addr;  QSNMPOid dstOid = oid;
    msg += ": addr " + QString::number(index.addr);

    switch (index.type) {
    case DType::Line: oid << 1;  msg += " line "; break;
    case DType::Loop: oid << 2; msg += " loop "; break;
    case DType::Output: oid << 3; msg += " output "; break;
    case DType::Group: oid << 4; msg += " group "; break;
    case DType::Dual: oid << 5; msg += " dual "; break;
    case DType::Env: oid << 6; msg += " env "; break;
    case DType::Sensor: oid << 7; msg += " Sensor ";  break;
    default: cout << index.type; break;
    } oid << index.id; msg += QString::number(index.id+1);

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
    default: cout << index.topic; break;
    }

//    if(index.topic == DTopic::Relay);
//    else if(index.topic == DType::Sensor);
//    else if(index.type == DType::Env) oid << 6;
//    else oid << 7;

    if(value) {
        QString doid = toString(dstOid << 0); // dstOid << 11   2
        sAgentCfg *cfg = &Agent_Core::snmpCfg;
        for(int i=0; i<SNMP_TRAP_SIZE; ++i) {
            QString ip = cfg->trap[i]; if(ip.size()) {
                sendTrap(ip, doid, toString(oid), msg);
                mSnmp->sendTrap(oid);
            }
        }

        //sendTrap("192.168.1.196", doid, toString(oid), msg);
    }
}

//  V2 命令  版本   -c 共同体  管理机  Enterprise-OID  snmp代理地址   陷阱类型 oid   数据类型    数据类型
void Agent_Trap::sendTrap(const QString &ip, const QString &dstOid, const QString &oid, const QString &msg)
{
    sAgentCfg *cfg = &Agent_Core::snmpCfg;
    if(snmpCfg.enV3) {
        QString fmd = "snmptrap -v 3 -a MD5 -A %1 -x %2 -X %3 -u %4 %5 '' %6 %7 s '%8'";
        QString cmd = fmd.arg(cfg->pwd, cfg->encrypt?"AES ":"DES", cfg->key, cfg->usr,ip, dstOid, oid, msg);
        system(cmd.toLatin1().data());
    } else  {
        QString cmd = "snmptrap -v 2c -c %5 %1 '' %2 %3 s '%4'";
        QString str = cmd.arg(ip, dstOid, oid, msg, cfg->get);
        system(str.toLatin1().data());
    }
}
