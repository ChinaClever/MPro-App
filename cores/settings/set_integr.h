#ifndef SET_INTEGR_H
#define SET_INTEGR_H
#include "set_output.h"

class Set_Integr : public Set_Output
{
public:
    Set_Integr();
protected:
    int modbusCfg(uchar fc);
    bool modbusSet(uchar fc, int value, uchar txType);

    QString snmpCfg(uchar fc);
    bool snmpSet(uchar fc, const QString &value, uchar txType);

    int rpcCfg(uchar fc);
    bool rpcSet(uchar fc, int value, uchar txType);

    QVariant pushCfg(uchar fc);
    bool pushSet(uchar fc, QVariant value, uchar txType);

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
