#ifndef AGENT_DEVDATA_H
#define AGENT_DEVDATA_H

#include "agent_object.h"

class Agent_DevData : public Agent_Object
{
    Q_OBJECT
public:
    explicit Agent_DevData(QObject *parent = nullptr);

    void addOids();
signals:

private:

    void addUutInfo(uchar addr, const QString &oidPrefix, sUutInfo &it);
    void addDevInfo(uchar addr, const QString &oidPrefix, sDevInfo &dev);

    void addAlarmUnit(uchar addr, uchar key, const QString &oidPrefix, const QString &oidName, sAlarmUnit &it, int index);
    void addObjData(uchar addr, const QString &oidPrefix, const QString &oidName, sObjData &it, int index);

    void addEnvData(uchar addr, uchar key, const QString &oidPrefix, sEnvData &it);
    void addDevData(uchar addr, sDevData *it);
};

#endif // AGENT_DEVDATA_H
