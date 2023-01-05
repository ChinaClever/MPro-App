/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_get.h"

Agent_Get::Agent_Get(QObject *parent) : Agent_Obj{parent}
{
    if((snmpCfg.enV3) || (snmpCfg.enV2)) {
        QTimer::singleShot(5255,this,SLOT(addOidSlot()));
    }
}

void Agent_Get::addUutInfo(uchar addr, const QString &oidPrefix, sUutInfo &it)
{
    int id = 1; QString oid = "0.0.";
    QString prefix = oidPrefix + "uut-";
    addOid(addr, id++, oid, prefix+"room", it.room);
    addOid(addr, id++, oid, prefix+"location", it.location);
    addOid(addr, id++, oid, prefix+"name", it.devName);
    addOid(addr, id++, oid, prefix+"qrcode", it.qrcode);
    addOid(addr, id++, oid, prefix+"sn", it.sn);
    addOid(addr, id++, oid, prefix+"dev", it.devType);
}

void Agent_Get::addDevNums(uchar addr, const QString &oidPrefix, sDevNums &dev)
{
    bool w = false; QString oid = "0.1.";
    int id = 1; QString prefix = oidPrefix + "num-";
    addOidValue(addr, id++, oid, prefix+"line", dev.lineNum, w);
    addOidValue(addr, id++, oid, prefix+"loop", dev.loopNum, w);
    addOidValue(addr, id++, oid, prefix+"output", dev.outputNum, w);
    addOidValue(addr, id++, oid, prefix+"board", dev.boardNum, w);
    addOidValue(addr, id++, oid, prefix+"slave", dev.slaveNum, w);
}

void Agent_Get::addDevParam(uchar addr, const QString &oidPrefix, sParameter &dev)
{
    bool w = false; QString oid = "0.2.";
    int id = 1; QString prefix = oidPrefix + "param-";
    addOidValue(addr, id++, oid, prefix+"spec", dev.devSpec, w);
    addOidValue(addr, id++, oid, prefix+"status", dev.runStatus, w);
    addOidValue(addr, id++, oid, prefix+"mode", dev.devMode, w);
    addOidValue(addr, id++, oid, prefix+"addr", dev.cascadeAddr, w);
    addOidValue(addr, id++, oid, prefix+"runTime", dev.runTime, w);
    addOidValue(addr, id++, oid, prefix+"hz", dev.hz, w);
    addOidValue(addr, id++, oid, prefix+"buzzer", dev.buzzerSw, w);
    addOidValue(addr, id++, oid, prefix+"group", dev.groupEn, w);
    addOidValue(addr, id++, oid, prefix+"sensorBox", dev.sensorBoxEn, w);
    addOidValue(addr, id++, oid, prefix+"language", dev.language, w);
    addOidValue(addr, id++, oid, prefix+"breaker", dev.isBreaker, w);
}

void Agent_Get::addDevVer(uchar addr, const QString &oidPrefix, sVersions &dev)
{
    bool w = false; QString oid = "0.3.";
    int id = 1; QString prefix = oidPrefix + "fw-";
    addOid(addr, id++, oid, prefix+"version", dev.ver, w);
    addOid(addr, id++, oid, prefix+"compileTime", dev.compileDate, w);
    addOid(addr, id++, oid, prefix+"software-md5", dev.md5, w);
    addOid(addr, id++, oid, prefix+"releaseDate", dev.releaseDate, w);
    addOid(addr, id++, oid, prefix+"upgradeDate", dev.upgradeDate, w);
    addOid(addr, id++, oid, prefix+"serialNumber", dev.serialNumber, w);

    for(int i=0; i<DEV_NUM; ++i) {
        prefix = oidPrefix + "fw-opver-" + QString::number(i+1);
        addOidValue(addr, 11+i, oid, prefix, dev.opVers[i], w);
    }
}

void Agent_Get::addDevInfo(uchar addr, const QString &oidPrefix, sDevCfg &dev)
{
    QString oid = oidPrefix + "info-";
    addUutInfo(addr, oid, dev.uut);
    addDevNums(addr, oid, dev.nums);
    addDevParam(addr, oid, dev.param);
    addDevVer(addr, oid, dev.vers);
}

void Agent_Get::addAlarmUnit(uchar addr, uchar key, const QString &oidPrefix,
                             const QString &oidName, sAlarmUnit &it, int index)
{
    int id = 1; QString name = oidName + "-";
    QString oid = oidPrefix + QString::number(key) +".";
    addOidValue(addr, id++, oid, name+"value", it.value[index], false);
    addOidValue(addr, id++, oid, name+"alarm-status", it.alarm[index], false);

    addOidValue(addr, id++, oid, name+"alarm-max", it.max[index]);
    addOidValue(addr, id++, oid, name+"warn-max", it.crMax[index]);

    addOidValue(addr, id++, oid, name+"warn-min", it.crMin[index]);
    addOidValue(addr, id++, oid, name+"alarm-min", it.min[index]);
    addOidValue(addr, id++, oid, name+"alarm-enable", it.en[index]);
    addOidValue(addr, id++, oid, name+"rated", it.rated[index], false);
}

void Agent_Get::addRelayUnit(uchar addr, uchar key, const QString &oidPrefix,
                            const QString &oidName, sRelayUnit &it, int index)
{
    int id = 1; QString name = oidName + "-";
    QString oid = oidPrefix + QString::number(key) +".";
    addOidValue(addr, id++, oid, name+"switch", it.sw[index], false);
    addOidValue(addr, id++, oid, name+"alarm-status", it.alarm[index], false);
    addOidValue(addr, id++, oid, name+"off-alarm", it.offAlarm[index]);
    addOidValue(addr, id++, oid, name+"up-delay", it.powerUpDelay[index]);
    addOidValue(addr, id++, oid, name+"reset-delay", it.resetDelay[index]);
    addOidValue(addr, id++, oid, name+"overrun-off", it.overrunOff[index]);
    addOidValue(addr, id++, oid, name+"enable", it.en[index]);
}

void Agent_Get::addObjData(uchar addr, const QString &oidPrefix,
                           const QString &oidName, sObjData &it, int index, int relay)
{
    int id = 0; QString oid = oidPrefix + ".";
    QString name = tr("%1-%2-").arg(oidName).arg(index+1);
    if(strlen((const char *)it.name)) addOid(addr, id++, oid, name+"name", it.name[index]); else id++;    
    if(it.relay.size){
        if(2 == relay) addOidValue(addr, id++, oid, name+"breaker", it.relay.sw[index]);
        else if(1 == relay) addRelayUnit(addr, id++, oid, name+"relay", it.relay, index); else id++;
    } else id++;

    if(it.vol.size) addAlarmUnit(addr, id++, oid, name+"vol", it.vol, index); else id++;
    if(it.cur.size) addAlarmUnit(addr, id++, oid, name+"cur", it.cur, index); else id++;
    addAlarmUnit(addr, id++, oid, name+"pow", it.pow, index);
    addOidValue(addr, id++, oid, name+"ele", it.ele[index], false);
    addOidValue(addr, id++, oid, name+"pf", it.pf[index], false);
}

void Agent_Get::addEnvData(uchar addr, const QString &oidPrefix,
                           const QString &oidName, sEnvData &it, int index)
{
    int id = 6; QString oid = oidPrefix + ".";
    QString name = tr("%1-%2-").arg(oidName).arg(index+1);
    addAlarmUnit(addr, id++, oid, name+"tem", it.tem, index);
    addAlarmUnit(addr, id++, oid, name+"hum", it.hum, index);
}


void Agent_Get::addDoors(uchar addr, const QString &oidPrefix, sEnvData &it)
{
    int id = 1; QString oid = "7.";
    QString prefix = oidPrefix + "sensor-";
    addOidValue(addr, id++, oid, prefix+"door-one", it.door[0], false);
    addOidValue(addr, id++, oid, prefix+"door-two", it.door[1], false);
    addOidValue(addr, id++, oid, prefix+"smoke", it.smoke[0], false);
    addOidValue(addr, id++, oid, prefix+"water", it.water[0], false);
}


void Agent_Get::addDevData(uchar addr, sDevData *it)
{
    QString name = tr("pdu-%1-").arg(addr);
    addDevInfo(addr, name, it->cfg);

    int size = it->cfg.nums.lineNum; // if(!size) size = LINE_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "1." +QString::number(i+1);
        addObjData(addr, oid, name+"line", it->line, i);
    }

    size = it->cfg.nums.loopNum;  // if(!size) size = LOOP_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "2." +QString::number(i+1);
        addObjData(addr, oid, name+"loop", it->loop, i, 2);
    }

    size = it->cfg.nums.outputNum; //it->output.size;  if(!size) size = OUTPUT_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "3." +QString::number(i+1);
        addObjData(addr, oid, name+"output", it->output, i, 1);
    }

    size = it->group.size; // if(!size) size = GROUP_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "4." +QString::number(i+1);
        addObjData(addr, oid, name+"group", it->group, i);
    }

    size = it->dual.size; // if(!size) size = GROUP_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "5." +QString::number(i+1);
        addObjData(addr, oid, name+"dual", it->group, i);
    }

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
    } mMib->save();
}
