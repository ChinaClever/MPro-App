#ifndef AGENT_GET_H
#define AGENT_GET_H

#include "agent_obj.h"

class Agent_Get : public Agent_Obj
{
public:
    explicit Agent_Get(QObject *parent = nullptr);
    void addOidSlot();

private:
    void addUutInfo(uchar addr, const QString &oidPrefix, sUutInfo &it);
    void addDevNums(uchar addr, const QString &oidPrefix, sDevNums &dev);
    void addDevVer(uchar addr, const QString &oidPrefix, sVersions &dev);
    void addDevParam(uchar addr, const QString &oidPrefix, sParameter &dev);
    void addDevInfo(uchar addr, const QString &oidPrefix, sDevCfg &dev);

    void addUnitData(uchar addr, uchar key, const QString &oidPrefix, const QString &oidName, sAlarmUnit &it, int index);
    void addUnitAlarm(uchar addr, uchar key, const QString &oidPrefix, const QString &oidName, sAlarmUnit &it, int index);
    void addObjData(uchar addr, const QString &oidPrefix, const QString &oidName, sObjData &it, int index, int type=0);
    void addObjAlarm(uchar addr, const QString &oidPrefix, const QString &oidName, sObjData &it, int index, int type);

    void addOutputEle(uchar addr, const QString &oidPrefix, const QString &oidName, sObjData &it, int index);
    void addRelayGroup(uchar addr, const QString &oidPrefix, const QString &oidName, sRelayUnit &it, int index);
    void addRelayDual(uchar addr, const QString &oidPrefix, const QString &oidName, sRelayUnit &it, int index);
    void addRelayUnit(uchar addr, const QString &oidPrefix, const QString &oidName, sRelayUnit &it, int index);
    void addEnvData(uchar addr, const QString &oidPrefix, const QString &oidName, sEnvData &it, int index);
    void addEnvAlarm(uchar addr, const QString &oidPrefix, const QString &oidName, sEnvData &it, int index);
    void addDoors(uchar addr, const QString &oidPrefix, sEnvData &it);
    void addDevData(uchar addr, sDevData *it);
};

#endif // AGENT_GET_H
