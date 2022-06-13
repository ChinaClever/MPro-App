#ifndef SET_INFO_H
#define SET_INFO_H

#include "set_alarm.h"

class Set_Info : public Set_Alarm
{
public:
    explicit Set_Info();

protected:
    QString getUut(int addr, uchar fc);
    bool setUut(uchar fc, char *str, uchar txType);

    int devCfgNum(int addr, int type);
    bool setCfgNum(int addr, int type, int value);

    int devInfos(int addr, int type);
};

#endif // SET_INFO_H
