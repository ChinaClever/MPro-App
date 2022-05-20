#ifndef ALARM_OBJECT_H
#define ALARM_OBJECT_H
#include "sm_obj.h"

class Alarm_Object
{
public:
    Alarm_Object();

    char alarmValue(const sDIndex &index);
    char alarmValue(const sDIndex &index, AlarmStatus type);
    char alarmValue(const sDIndex &index, sRelay::Alarm type);

//protected:
    sTgUnit *getTgAlarmUnit(const sDIndex &index);
    sAlarmUnit *getAlarmUnit(const sDIndex &index);
    sRelayUnit *getRelayUnit(const sDIndex &index);
    uint *getValue(const sDIndex &index);

private:
    sObjData *getObjData(const sDIndex &index);
    sAlarmUnit *getAlarmUnit(const sDIndex &index, sObjData *obj);
    uint *getValueAlarmUnit(const sDIndex &index);
    uint *getValueTgAlarmUnit(const sDIndex &index);
    uint *getValueRelayUnit(const sDIndex &index);
};

#endif // ALARM_OBJECT_H
