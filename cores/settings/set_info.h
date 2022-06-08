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
};

#endif // SET_INFO_H
