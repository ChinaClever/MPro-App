#ifndef ALARM_OBJECT_H
#define ALARM_OBJECT_H
#include "sm_obj.h"

class Alarm_Object
{
public:
    Alarm_Object();

    char alarmValue(const sAlarmIndex &index);
    char alarmValue(const sAlarmIndex &index, AlarmType type);
    char alarmValue(const sAlarmIndex &index, sRelay::Alarm type);

//protected:
    sTgUnit *getTgAlarmUnit(const sAlarmIndex &index);
    sAlarmUnit *getAlarmUnit(const sAlarmIndex &index);
    sRelayUnit *getRelayUnit(const sAlarmIndex &index);

private:
    sObjData *getObjData(const sAlarmIndex &index);
    sAlarmUnit *getAlarmUnit(const sAlarmIndex &index, sObjData *obj);
};

#endif // ALARM_OBJECT_H
