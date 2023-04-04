#ifndef ALARM_OBJECT_H
#define ALARM_OBJECT_H
#include "shm.h"

class Alarm_Object
{
public:
    Alarm_Object();

    char alarmValue(const sDataItem &index);
    char alarmValue(const sDataItem &index, AlarmCode type);
    char alarmValue(const sDataItem &index, sRelay::Alarm type);

    sTgUnit *getTgAlarmUnit(const sDataItem &index);
    sAlarmUnit *getAlarmUnit(const sDataItem &index);

    sRelayUnit *getRelayUnit(const sDataItem &index);
    bool upMetaData(sDataItem &index);

private:
    sObjData *getObjData(const sDataItem &index);
    sAlarmUnit *getAlarmUnit(const sDataItem &index, sObjData *obj);
    bool alarmUnitCheck(sDataItem &index, sAlarmUnit *unit);
    bool setAll(uint *ptr, sDataItem &index, sAlarmUnit *unit);
    bool setAll(uint *ptr, uint value, int size);
    bool alarmUnitValue(sDataItem &index);
    bool tgAlarmUnitValue(sDataItem &index);
    bool tgValue(sDataItem &index);

    bool relayUnitValue(sDataItem &index);
    bool sensorValue(sDataItem &index);
    bool powPfValue(sDataItem &index);
    bool eleValue(sDataItem &index);
};

#endif // ALARM_OBJECT_H
