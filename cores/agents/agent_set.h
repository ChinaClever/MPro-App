#ifndef AGENT_SET_H
#define AGENT_SET_H

#include "agent_get.h"

class Agent_Set : public Agent_Get
{
public:
    explicit Agent_Set(QObject *parent = nullptr);
    void snmpSetSlot(uint addr, const QSNMPOid &oid, const QVariant &value);
    void initSetSlot();

private:
    bool toIndex(uint addr, const QSNMPOid &oid);
    bool paramSet(const QVariant &value);
    bool uutSet(const QVariant &value);
    bool cfgSet(const QVariant &value);

    bool upAlarmIndex(sDataItem &index);
    bool setAlarm(const QVariant &value);
    bool ctrlOutput(const QVariant &value);

    bool setName(int type,const QVariant &value);
    bool relayDualCtrl(const QVariant &value);
    bool relayGroupCtrl(const QVariant &value);
    bool setOutputName(const QVariant &value);


private:
    struct sIndex {
        uchar addr;
        uchar rw;
        uchar fc;
        uchar id;
        uchar type;
        uchar subtopic;
    }mIndex;
    QSNMPOid m_oid;
};

#endif // AGENT_SET_H
