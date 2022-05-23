#ifndef SET_ALARM_H
#define SET_ALARM_H

#include "set_unserialize.h"

class Set_Alarm : public Alarm_Object
{
public:
    explicit Set_Alarm();

protected:
    bool setAlarm(sDataItem &unit);
};

#endif // SET_ALARM_H
