#ifndef AGENT_GET_H
#define AGENT_GET_H

#include "agent_obj.h"

class Agent_Get : public Agent_Obj
{
    Q_OBJECT
public:
    explicit Agent_Get(QObject *parent = nullptr);

private slots:
    void addOidSlot();

private:
    void addUutInfo(uchar addr, const QString &oidPrefix, sUutInfo &it);
    void addDevInfo(uchar addr, const QString &oidPrefix, sDevInfo &dev);

    void addAlarmUnit(uchar addr, uchar key, const QString &oidPrefix, const QString &oidName, sAlarmUnit &it, int index);
    void addObjData(uchar addr, const QString &oidPrefix, const QString &oidName, sObjData &it, int index);

    void addEnvAlarm(uchar addr, uchar key, const QString &oidPrefix, const QString &oidName, sAlarmUnit &it, int index);
    void addEnvData(uchar addr, const QString &oidPrefix, const QString &oidName, sEnvData &it, int index);
    void addDoors(uchar addr, const QString &oidPrefix, sEnvData &it);
    void addDevData(uchar addr, sDevData *it);
};

#endif // AGENT_GET_H
