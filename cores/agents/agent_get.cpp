/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_get.h"

Agent_Get::Agent_Get(QObject *parent) : Agent_Obj{parent}
{
    QTimer::singleShot(2750,this,SLOT(addOidSlot()));
}

void Agent_Get::addUutInfo(uchar addr, const QString &oidPrefix, sUutInfo &it)
{
    int id = 1; QString oid = "0.0.";
    QString prefix = oidPrefix + "uut_";

    addOid(addr, id++, oid, prefix+"idc", it.idc);
    addOid(addr, id++, oid, prefix+"room", it.room);
    addOid(addr, id++, oid, prefix+"modular", it.module);
    addOid(addr, id++, oid, prefix+"cabinet", it.cab);
    addOid(addr, id++, oid, prefix+"road", it.road);
    addOid(addr, id++, oid, prefix+"name", it.devName);
}

void Agent_Get::addDevInfo(uchar addr, const QString &oidPrefix, sDevInfo &dev)
{
    bool w = false; QString oid = "0.1.";
    int id = 1; QString prefix = oidPrefix + "info_";

    addOidValue(addr, id++, oid, prefix+"type", dev.devSpec, w);
    addOidValue(addr, id++, oid, prefix+"lineNum", dev.lineNum, w);
    addOidValue(addr, id++, oid, prefix+"outputNum", dev.outputNum, w);
    addOidValue(addr, id++, oid, prefix+"slaveNum", dev.slaveNum, w);
    addOidValue(addr, id++, oid, prefix+"version", dev.version, w);
    addOidValue(addr, id++, oid, prefix+"hz", dev.hz, w);
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
    addOidValue(addr, id++, oid, name+"pf", it.pf[index]);
    addOidValue(addr, id++, oid, name+"ele", it.ele[index]);
}

void Agent_Get::addEnvAlarm(uchar addr, uchar key, const QString &oidPrefix,
                            const QString &oidName, sAlarmUnit &it, int index)
{
    int id = 1; QString name = oidName + "_";
    QString oid = oidPrefix + QString::number(key) +".";
    addOidValue(addr, id++, oid, name+"value", it.value[index], false);
    addOidValue(addr, id++, oid, name+"alarm_max", it.max[index]);
    addOidValue(addr, id++, oid, name+"alarm_min", it.min[index]);
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

    addUutInfo(addr, name, it->uut);
    addDevInfo(addr, name, it->info);

    int size = it->line.size; // if(!size) size = LINE_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "1." +QString::number(i+1);
        addObjData(addr, oid, name+"line", it->line, i);
    }

    size = it->loop.size;  // if(!size) size = LOOP_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "2." +QString::number(i+1);
        addObjData(addr, oid, name+"loop", it->loop, i);
    }

    size = it->output.size; // if(!size) size = OUTPUT_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "3." +QString::number(i+1);
        addObjData(addr, oid, name+"output", it->output, i);
    }

    size = it->env.size; // if(!size) size = SENOR_NUM;
    for(int i=0; i<size; ++i) {
        QString oid = "6." + QString::number(i+1);
        addEnvData(addr, oid, name+"env", it->env, i);
    } addDoors(addr, name, it->env);
}

void Agent_Get::addOidSlot()
{
    uint size = cm::masterDev()->info.slaveNum;
    for(uint i=0; i<=size; ++i) {
        sDevData *dev = cm::devData(i);
        addDevData(i, dev);
    }
}
