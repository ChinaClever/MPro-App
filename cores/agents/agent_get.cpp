/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_get.h"

Agent_Get::Agent_Get(QObject *parent) : Agent_Obj{parent}
{
    if((snmpCfg.enV3) || (snmpCfg.enV2)) {
        QTimer::singleShot(5255,this,&Agent_Get::addOidSlot);
    }
}

void Agent_Get::addUutInfo(uchar addr, const QString &oidPrefix, sUutInfo &it)
{
    int id = 1; QString oid = "2.0.1.";
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
    bool w = false; QString oid = "1.0.1.";
    int id = 1; QString prefix = oidPrefix + "num-";
    addOidValue(addr, id++, oid, prefix+"line", dev.lineNum, w);
    addOidValue(addr, id++, oid, prefix+"loop", dev.loopNum, w);
    addOidValue(addr, id++, oid, prefix+"output", dev.outputNum, w);
    addOidValue(addr, id++, oid, prefix+"board", dev.boardNum, w);
    addOidValue(addr, id++, oid, prefix+"slave", dev.slaveNum, w);
}

void Agent_Get::addDevParam(uchar addr, const QString &oidPrefix, sParameter &dev)
{
    bool w = false; QString oid = "1.0.2.";
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
    bool w = false; QString oid = "1.0.3.";
    int id = 1; QString prefix = oidPrefix + "read-fw-";
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
    QString oid = oidPrefix + "rw-info-";
    addUutInfo(addr, oid, dev.uut);

    oid = oidPrefix + "read-info-";
    addDevNums(addr, oid, dev.nums);
    addDevParam(addr, oid, dev.param);
    addDevVer(addr, oid, dev.vers);
}

void Agent_Get::addUnitAlarm(uchar addr, uchar key, const QString &oidPrefix,
                             const QString &oidName, sAlarmUnit &it, int index)
{
    int id = 1; QString name = oidName + "-";
    QString oid = oidPrefix + QString::number(key) +".";
    addOidValue(addr, id++, oid, name+"alarm-enable", it.en[index]);
    addOidValue(addr, id++, oid, name+"alarm-max", it.max[index]);
    addOidValue(addr, id++, oid, name+"warn-max", it.crMax[index]);
    addOidValue(addr, id++, oid, name+"warn-min", it.crMin[index]);
    addOidValue(addr, id++, oid, name+"alarm-min", it.min[index]);
    addOidValue(addr, id++, oid, name+"rated", it.rated[index], false);
}

void Agent_Get::addUnitData(uchar addr, uchar key, const QString &oidPrefix,
                             const QString &oidName, sAlarmUnit &it, int index)
{
    int id = 1; QString name = oidName + "-";
    QString oid = oidPrefix + QString::number(key) +".";
    addOidValue(addr, id++, oid, name+"value", it.value[index], false);
    addOidValue(addr, id++, oid, name+"alarm-status", it.alarm[index], false);
}

void Agent_Get::addRelayUnit(uchar addr, const QString &oidPrefix,
                            const QString &oidName, sRelayUnit &it, int index)
{
    if(!it.size) return;
    int id = 1; QString oid = oidPrefix + ".1.";
    QString name = tr("%1-%2-relay-").arg(oidName).arg(index+1);
    addOidValue(addr, id++, oid, name+"switch", it.sw[index]);
    addOidValue(addr, id++, oid, name+"disabled", it.disabled[index]);
    addOidValue(addr, id++, oid, name+"alarm-status", it.alarm[index], false);
    addOidValue(addr, id++, oid, name+"off-alarm", it.offAlarm[index]);
    addOidValue(addr, id++, oid, name+"up-delay", it.powerUpDelay[index]);
    addOidValue(addr, id++, oid, name+"reset-delay", it.resetDelay[index]);
    addOidValue(addr, id++, oid, name+"overrun-off", it.overrunOff[index]);
}

void Agent_Get::addRelayGroup(uchar addr, const QString &oidPrefix,
                            const QString &oidName, sRelayUnit &it, int index)
{
    if(!it.size) return;
    int id = 1; QString oid = oidPrefix + ".1.";
    QString name = tr("%1-%2-relay-").arg(oidName).arg(index+1);
    addOidValue(addr, id++, oid, name+"switch", it.sw[index]);
}

void Agent_Get::addOutputEle(uchar addr, const QString &oidPrefix,
                            const QString &oidName, sObjData &it, int index)
{
    if(!it.size) return;
    int id = 1; QString oid = oidPrefix + ".2.";
    QString name = tr("%1-%2-ele-").arg(oidName).arg(index+1);
    addOidValue(addr, id++, oid, name+"value", it.ele[index]);
}

void Agent_Get::addObjData(uchar addr, const QString &oidPrefix,
                           const QString &oidName, sObjData &it, int index, int type)
{
    int id = 1; QString oid = oidPrefix + ".";
    QString name = tr("%1-%2-").arg(oidName).arg(index+1);
    if(it.vol.size) addUnitData(addr, id++, oid, name+"vol", it.vol, index);
    if(it.cur.size) addUnitData(addr, id++, oid, name+"cur", it.cur, index);
    if(it.pow.size) addUnitData(addr, id++, oid, name+"pow", it.pow, index);
    addOidValue(addr, id++, oid, name+"artpow", it.artPow[index], false);
    addOidValue(addr, id++, oid, name+"pf", it.pf[index], false);
    addOidValue(addr, id++, oid, name+"ele", it.ele[index], false);
    if(it.vol.size > 1 && (1 == type)) addOidValue(addr, id++,oid, name+"phase-vol", it.lineVol[index], false);
    if(it.relay.size && (2 == type))addOidValue(addr, id++, oid, name+"breaker", it.relay.sw[index], false);
}

void Agent_Get::addObjAlarm(uchar addr, const QString &oidPrefix,
                           const QString &oidName, sObjData &it, int index, int type)
{
    int id = 0; QString oid = oidPrefix + ".";
    QString name = tr("%1-%2-").arg(oidName).arg(index+1);
    if(type > 2) addOid(addr, id++, oid, name+"name", it.name[index]); else id++;
    if(it.vol.size) addUnitAlarm(addr, id++, oid, name+"vol", it.vol, index);
    if(it.cur.size) addUnitAlarm(addr, id++, oid, name+"cur", it.cur, index);
    if(it.pow.size) addUnitAlarm(addr, id++, oid, name+"pow", it.pow, index);
}

void Agent_Get::addEnvData(uchar addr, const QString &oidPrefix,
                           const QString &oidName, sEnvData &it, int index)
{
    int id = 1; QString oid = oidPrefix + ".";
    QString name = tr("%1-%2-").arg(oidName).arg(index+1);
    addUnitData(addr, id++, oid, name+"tem", it.tem, index);
    addUnitData(addr, id++, oid, name+"hum", it.hum, index);
}

void Agent_Get::addEnvAlarm(uchar addr, const QString &oidPrefix,
                           const QString &oidName, sEnvData &it, int index)
{
    int id = 1; QString oid = oidPrefix + ".";
    QString name = tr("%1-%2-").arg(oidName).arg(index+1);
    addUnitAlarm(addr, id++, oid, name+"tem", it.tem, index);
    addUnitAlarm(addr, id++, oid, name+"hum", it.hum, index);
}

void Agent_Get::addDoors(uchar addr, const QString &oidPrefix, sEnvData &it)
{
    int id = 1; QString oid = "1.7.";
    QString prefix = oidPrefix + "read-sensor-";
    addOidValue(addr, id++, oid, prefix+"door-one", it.door[0], false);
    addOidValue(addr, id++, oid, prefix+"door-two", it.door[1], false);
    addOidValue(addr, id++, oid, prefix+"smoke", it.smoke[0], false);
    addOidValue(addr, id++, oid, prefix+"water", it.water[0], false);
}


void Agent_Get::addDevData(uchar addr, sDevData *it)
{
    Agent_Mib::bulid()->initModule();
    QString name = tr("pdu-%1-").arg(addr);
    addDevInfo(addr, name, it->cfg);

    int size = it->cfg.nums.lineNum;
    for(int i=0; i<size; ++i) {
        QString oid = "1.1." +QString::number(i+1);
        addObjData(addr, oid, name+"read-line", it->line, i, 1);

        oid = "2.1." +QString::number(i+1);
        addObjAlarm(addr, oid, name+"rw-line", it->line, i, 1);
    }

    size = it->cfg.nums.loopNum;
    for(int i=0; i<size; ++i) {
        QString oid = "1.2." +QString::number(i+1);
        addObjData(addr, oid, name+"read-loop", it->loop, i, 2);

        oid = "2.2." +QString::number(i+1);
        addObjAlarm(addr, oid, name+"rw-loop", it->loop, i, 2);
    }

    size = it->cfg.nums.outputNum;
    for(int i=0; i<size; ++i) {
        QString oid = "1.3." +QString::number(i+1);
        addObjData(addr, oid, name+"read-output", it->output, i, 3);

        oid = "2.3." +QString::number(i+1);
        addObjAlarm(addr, oid, name+"rw-output", it->output, i, 3);

        oid = "3.1." +QString::number(i+1);
        addRelayUnit(addr, oid, name+"ctrl-output", it->output.relay, i);
        addOutputEle(addr, oid, name+"ctrl-output", it->output, i);
    }

    size = it->group.size;
    for(int i=0; i<size; ++i) {
        QString oid = "1.4." +QString::number(i+1);
        addObjData(addr, oid, name+"read-group", it->group, i, 4);

        oid = "2.4." +QString::number(i+1);
        addObjAlarm(addr, oid, name+"rw-group", it->group, i, 4);

        //oid = "3.2." +QString::number(i+1);
        //addRelayGroup(addr, oid, name+"ctrl-group", it->output.relay, i);
    }

    size = it->output.relay.size;
    if(size) {
        for(int i=0; i<size; ++i) {
            QString oid = "3.2." +QString::number(i+1);
            addRelayGroup(addr, oid, name+"ctrl-group", it->output.relay, i);
        }
    }

    size = it->dual.size;
    for(int i=0; i<size; ++i) {
        QString oid = "1.5." +QString::number(i+1);
        addObjData(addr, oid, name+"read-dual", it->group, i);

        oid = "2.5." +QString::number(i+1);
        addObjAlarm(addr, oid, name+"rw-dual", it->group, i, 5);

        oid = "3.3." +QString::number(i+1);
        addRelayGroup(addr, oid, name+"ctrl-dual", it->output.relay, i);
    }


    size = it->env.size;if(!size) size = SENOR_NUM/2;
    for(int i=0; i<size; ++i) {
        QString oid = "1.6." + QString::number(i+1);
        addEnvData(addr, oid, name+"read-env", it->env, i);

        oid = "2.6." + QString::number(i+1);
        addEnvAlarm(addr, oid, name+"rw-env", it->env, i);
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
