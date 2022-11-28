#ifndef SET_INTEGR_H
#define SET_INTEGR_H
#include "set_sercret.h"

class Set_Integr : public Set_Sercret
{
public:
    Set_Integr();
protected:
    int modbusCfg(uchar fc);
    bool modbusSet(uchar fc, int value);

    QVariant snmpCfg(uchar fc, int id);
    bool snmpSet(uchar fc, int id, const QVariant &v);

    int rpcCfg(uchar fc);
    bool rpcSet(uchar fc, int value);

    QVariant pushCfg(uchar fc, int id);
    bool pushSet(uchar fc, int id, const QVariant &v);

    QVariant mqttCfg(uchar fc);
    bool mqttSet(uchar fc, const QVariant &v);

    QVariant amqpCfg(uchar fc);
    bool amqpSet(uchar fc, const QVariant &v);

    QVariant odbcCfg(uchar fc);
    bool odbcSet(uchar fc, const QVariant &v);

    QVariant redisCfg(uchar fc);
    bool redisSet(uchar fc, const QVariant &v);

private:
    int toBaud(int value);
    int baudTo(int value);

    int parityTo(int value);
    int toParity(int value);

    int dataBitsTo(int value);
    int toDataBits(int value);

    int stopBitsTo(int value);
    int toStopBits(int value);
};

#endif // SET_INTEGR_H
