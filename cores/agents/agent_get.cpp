/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_get.h"

Agent_Get::Agent_Get(QObject *parent) : Agent_Obj{parent}
{
    if((snmpCfg.enV3) || (snmpCfg.enV2)) {
        QTimer::singleShot(3255,this,SLOT(addOidSlot()));
    }
}

void Agent_Get::addUutInfo(uchar addr, const QString &oidPrefix, sUutInfo &it)
{
    int id = 1; QString oid = "0.0.";
    QString prefix = oidPrefix + "uut_";

    addOid(addr, id++, oid, prefix+"room", it.room);
    addOid(addr, id++, oid, prefix+"location", it.location);
    addOid(addr, id++, oid, prefix+"name", it.devName);
    addOid(addr, id++, oid, prefix+"qrcode", it.qrcode);
    addOid(addr, id++, oid, prefix+"sn", it.sn);
}

void Agent_Get::addDevNums(uchar addr, const QString &oidPrefix, sDevNums &dev)
{
    bool w = false; QString oid = "0.1.";
    int id = 1; QString prefix = oidPrefix + "num_";
    addOidValue(addr, id++, oid, prefix+"line", dev.lineNum, w);
    addOidValue(addr, id++, oid, prefix+"loop", dev.loopNum, w);
    addOidValue(addr, id++, oid, prefix+"output", dev.outputNum, w);
    addOidValue(addr, id++, oid, prefix+"slave", dev.slaveNum, w);
    addOidValue(addr, id++, oid, prefix+"board", dev.boardNum, w);
}

void Agent_Get::addDevParam(uchar addr, const QString &oidPrefix, sParameter &dev)
{
    bool w = false; QString oid = "0.2.";
    int id = 1; QString prefix = oidPrefix + "info_";
    addOidValue(addr, id++, oid, prefix+"type", dev.devSpec, w);
    addOidValue(addr, id++, oid, prefix+"hz", dev.hz, w);
    //addOidValue(addr, id++, oid, prefix+"group", dev.groupEn, w);
}

void Agent_Get::addDevVer(uchar addr, const QString &oidPrefix, sVersions &dev)
{
    bool w = false; QString oid = "0.3.";
    int id = 1; QString prefix = oidPrefix + "ver_";
    addOid(addr, id++, oid, prefix+"version", dev.ver, w);
    addOid(addr, id++, oid, prefix+"compileTime", dev.compileDate, w);
}

void Agent_Get::addDevInfo(uchar addr, const QString &oidPrefix, sDevCfg &dev)
{
    addUutInfo(addr, oidPrefix, dev.uut);
    addDevNums(addr, oidPrefix, dev.nums);
    addDevParam(addr, oidPrefix, dev.param);
    addDevVer(addr, oidPrefix, dev.vers);
}

void Agent_Get::addAlarmUnit(uchar addr, uchar key, const QString &oidPrefix,
                             const QString &oidName, sAlarmUnit &it, int index)
{
    int id = 1; QString name = oidName + "_";
    QString oid = oidPrefix + QString::number(key) +".";
    addOidValue(addr, id++, oid, name+"value", it.value[index], false);
    addOidValue(addr, id++, oid, name+"rated", it.rated[index], false);

    addOidValue(addr, id++, oid, name+"alarm_max", it.max[index]);
    addOidValue(addr, id++, oid, name+"warn_max", it.crMax[index]);

    addOidValue(addr, id++, oid, name+"warn_min", it.crMin[index]);
    addOidValue(addr, id++, oid, name+"alarm_min", it.min[index]);
    addOidValue(addr, id++, oid, name+"alarm_enable", it.en[index]);
    addOidValue(addr, id++, oid, name+"alarm_status", it.alarm[index], false);
}

void Agent_Get::addObjData(uchar addr, const QString &oidPrefix,
                           const QString &oidName, sObjData &it, int index)
{
    int id = 0; QString oid = oidPrefix + ".";
    QString name = tr("%1_%2_").arg(oidName).arg(index+1);
    if(strlen((const char *)it.name)) addOid(addr, id++, oid, name+"name", it.name[index]); else id++;
    if(it.relay.size) addOidValue(addr, id++, oid, name+"switch", it.relay.sw[index]); else id++;
    if(it.vol.size) addAlarmUnit(addr, id++, oid, name+"vol", it.vol, index); else id++;

    addAlarmUnit(addr, id++, oid, name+"cur", it.cur, index);
    addAlarmUnit(addr, id++, oid, name+"pow", it.pow, index);
    addOidValue(addr, id++, oid, name+"ele", it.ele[index], false);
    addOidValue(addr, id++, oid, name+"pf", it.pf[index], false);
}

void Agent_Get::addEnvAlarm(uchar addr, uchar key, const QString &oidPrefix,
                            const QString &oidName, sAlarmUnit &it, int index)
{
    int id = 1; QString name = oidName + "_";
    QString oid = oidPrefix + QString::number(key) +".";
    addOidValue(addr, id++, oid, name+"value", it.value[index], false);
    addOidValue(addr, id++, oid, name+"alarm_max", it.max[index]);
    addOidValue(addr, id++, oid, name+"warn_max", it.crMax[index]);

    addOidValue(addr, id++, oid, name+"warn_min", it.crMin[index]);
    addOidValue(addr, id++, oid, name+"alarm_min", it.min[index]);
    addOidValue(addr, id++, oid, name+"alarm_enable", it.en[index]);
    addOidValue(addr, id++, oid, name+"alarm_status", it.alarm[index], false);
}

void Agent_Get::addEnvData(uchar addr, const QString &oidPrefix,
                           const QString &oidName, sEnvData &it, int index)
{
    int id = 6; QString oid = oidPrefix + ".";
    QString name = tr("%1_%2_").arg(oidName).arg(index+1);
    addEnvAlarm(addr, id++, oid, name+"tem", it.tem, index);
    addEnvAlarm(addr, id++, oid, name+"hum", it.hum, index);
}


void Agent_Get::addDoors(uchar addr, const QString &oidPrefix, sEnvData &it)
{
    int id = 1; QString oid = "7.";
    QString prefix = oidPrefix + "sensor_";
    addOidValue(addr, id++, oid, prefix+"door_one", it.door[0], false);
    addOidValue(addr, id++, oid, prefix+"door_two", it.door[1], false);
    addOidValue(addr, id++, oid, prefix+"smoke", it.smoke[0], false);
    addOidValue(addr, id++, oid, prefix+"water", it.water[0], false);
}


void Agent_Get::addDevData(uchar addr, sDevData *it)
{
    QString name = tr("pdu_%1_").arg(addr);
    addDevInfo(addr, name, it->cfg);

    int size = it->cfg.nums.lineNum; // if(!size) size = LINE_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "1." +QString::number(i+1);
        addObjData(addr, oid, name+"line", it->line, i);
    }

    size = it->cfg.nums.loopNum;  // if(!size) size = LOOP_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "2." +QString::number(i+1);
        addObjData(addr, oid, name+"loop", it->loop, i);
    }

    size = it->cfg.nums.outputNum; //it->output.size;  if(!size) size = OUTPUT_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "3." +QString::number(i+1);
        addObjData(addr, oid, name+"output", it->output, i);
    }

    //size = it->group.size; // if(!size) size = GROUP_NUM;
    //for(int i=0; i<size; ++i) {
    //    QString oid = "4." +QString::number(i+1);
    //    addObjData(addr, oid, name+"group", it->group, i);
    //}

    size = it->env.size; if(!size) size = SENOR_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "6." + QString::number(i+1);
        addEnvData(addr, oid, name+"env", it->env, i);
    } addDoors(addr, name, it->env);
}

void Agent_Get::addOidSlot()
{
    uint size = cm::masterDev()->cfg.nums.slaveNum;
    for(uint i=0; i<=size; ++i) {
        sDevData *dev = cm::devData(i);
        addDevData(i, dev);
    }
}
