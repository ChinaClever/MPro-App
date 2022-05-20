#ifndef AGENT_SET_H
#define AGENT_SET_H

#include "agent_get.h"

class Agent_Set : public Agent_Get
{
    Q_OBJECT
public:
    explicit Agent_Set(QObject *parent = nullptr);

private slots:
    void initSetSlot();
    void snmpSetSlot(uint addr, const QSNMPOid &oid, const QVariant &value);

private:
    bool toIndex(uint addr, const QSNMPOid &oid);
    bool uutSet(const QVariant &value);

    bool setAlarmUnit(sSetAlarmUnit &unit, const QVariant &value);
    bool upAlarmIndex(sAlarmIndex &index);
    bool upAlarmData(sSetAlarmUnit &unit);
    bool setAlarm(const QVariant &value);

    bool relayCtrl(const QVariant &value);
    bool setOutputName(const QVariant &value);


private:
    struct sIndex {
        uchar addr;
        uchar fc;
        uchar id;
        uchar type;
        uchar subtopic;
    }mIndex;
};

#endif // AGENT_SET_H
