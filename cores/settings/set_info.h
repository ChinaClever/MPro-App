#ifndef SET_INFO_H
#define SET_INFO_H

#include "set_alarm.h"

class Set_Info : public Set_Alarm
{
public:
    explicit Set_Info();

protected:
    bool setUut(uchar fc, char *str);
    QString getUut(int addr, uchar fc);
};

#endif // SET_INFO_H
