#include "agent_devdata.h"

Agent_DevData::Agent_DevData(QObject *parent) : Agent_Object{parent}
{

}



void Agent_DevData::addUutInfo(uchar addr, const QString &oidPrefix, sUutInfo &it)
{
    int id = 1; QString prefix = oidPrefix + "uut_";
    QString oid = QString::number(addr) + ".0.0.";

    addOid(addr, id++, oid, prefix+"room", it.room);
    addOid(addr, id++, oid, prefix+"modular", it.module);
    addOid(addr, id++, oid, prefix+"cabinet", it.cab);
    addOid(addr, id++, oid, prefix+"road", it.road);
}

void Agent_DevData::addDevInfo(uchar addr, const QString &oidPrefix, sDevInfo &dev)
{
    int id = 1; QString prefix = oidPrefix + "info_";
    QString oid = QString::number(addr) + ".0.1.";

    addOid(addr, id++, oid, prefix+"name", dev.devName);
    addOidValue(addr, id++, oid, prefix+"type", dev.devSpec);
    addOidValue(addr, id++, oid, prefix+"phases", dev.phases);
    addOidValue(addr, id++, oid, prefix+"outputs", dev.outputs);
    addOidValue(addr, id++, oid, prefix+"slaveNum", dev.slaveNum);
    addOidValue(addr, id++, oid, prefix+"version", dev.version);
    addOidValue(addr, id++, oid, prefix+"hz", dev.hz);
}



void Agent_DevData::addAlarmUnit(uchar addr, uchar key, const QString &oidPrefix,
                                 const QString &oidName, sAlarmUnit &it, int index)
{
    int id = 1; QString name = oidName + "_";
    QString oid = oidPrefix + QString::number(key) +".";
    addOidValue(addr, id++, oid, name+"value", it.value[index]);
    addOidValue(addr, id++, oid, name+"rated", it.rated[index]);

    addOidValue(addr, id++, oid, name+"alarm_min", it.min[index]);
    addOidValue(addr, id++, oid, name+"warn_min", it.crMin[index]);

    addOidValue(addr, id++, oid, name+"warn_max", it.crMax[index]);
    addOidValue(addr, id++, oid, name+"alarm_max", it.max[index]);
}

void Agent_DevData::addObjData(uchar addr, const QString &oidPrefix,
                               const QString &oidName, sObjData &it, int index)
{
    int id = 1; QString oid = oidPrefix + ".";
    QString name = tr("%1_%2_").arg(oidName).arg(index+1);

    addOid(addr, id++, oid, name+"name", it.name[index]);
    addOidValue(addr, id++, oid, name+"switch", it.relay.sw[index]);

    addAlarmUnit(addr, id++, oid, name+"vol", it.vol, index);
    addAlarmUnit(addr, id++, oid, name+"cur", it.cur, index);
    addAlarmUnit(addr, id++, oid, name+"pow", it.pow, index);

    addOidValue(addr, id++, oid, name+"pf", it.pf[index]);
    addOidValue(addr, id++, oid, name+"ele", it.ele[index]);
}

void Agent_DevData::addEnvData(uchar addr, uchar key, const QString &oidPrefix, sEnvData &it)
{
    //    QString prefix = oidPrefix + "env_";

    //    addAlarmUnit(addr, id++, oid, name+"vol", it.vol, index);

}

void Agent_DevData::addDevData(uchar addr, sDevData *it)
{
    QString oidPrefix = QString::number(addr) +".";
    QString name = tr("pdu_%1_").arg(addr);

    addUutInfo(addr, name, it->uut);
    addDevInfo(addr, name, it->info);


    for(int i=0; i<LINE_NUM; ++i) {
        QString oid = oidPrefix + "1." +QString::number(i+1);
        addObjData(addr, oid, name+"line", it->line, i);
    }

    for(int i=0; i<LOOP_NUM; ++i) {
        QString oid = oidPrefix + "2." +QString::number(i+1);
        addObjData(addr, oid, name+"loop", it->loop, i);
    }

    for(int i=0; i<OUTPUT_NUM; ++i) {
        QString oid = oidPrefix + "3." +QString::number(i+1);
        addObjData(addr, oid, name+"output", it->output, i);
    }
}

void Agent_DevData::addOids()
{
    sDevData *dev = cm::devData(0);
    addDevData(8, dev);
}
