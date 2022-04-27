#ifndef SET_ALARM_H
#define SET_ALARM_H

#include "set_unserialize.h"

class Set_Alarm : public Alarm_Object
{
public:
    explicit Set_Alarm();
    bool setAlarm(const sSetAlarmUnit &unit);

private:
    bool setAlarmIt(const sSetAlarmUnit &unit);
    bool setTgAlarmUnit(const sSetAlarmUnit &unit);
    void setAlarmUnit(int id, sAlarmUnit *it, const sSetAlarmUnit &unit);

protected:
};

#endif // SET_ALARM_H
