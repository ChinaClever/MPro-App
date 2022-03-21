#ifndef ALARM_OBJECT_H
#define ALARM_OBJECT_H
#include "sm_obj.h"

enum AlarmType{Ok, Min=1, CrMin=2, CrMax=4, Max=8};
enum AlarmIndex{Tg, Line, Loop, Output, Vol, Cur, Pow, Relay, Env, Tem,Hum};

struct sAlarmIndex
{
    sAlarmIndex():addr(0){}
    uchar addr;
    uchar type;
    uchar subtopic;
    uchar id;
};

struct sSetAlarmUnit
{
    sAlarmIndex index;
    uint rated;
    uint min;
    uint max;
    uint crMin;
    uint crMax;
};


struct sRelay
{
    enum State{Off, On};
    enum Type{Breaker, Relay};
    enum Mode{Standard, NormaOpen, NormaClose};
    enum Alarm{NoAlarm, OpenALarm, CloseAlarm};
};

class Alarm_Object
{
public:
    Alarm_Object();

    char alarmValue(const sAlarmIndex &index);
    char alarmValue(const sAlarmIndex &index, AlarmType type);
    char alarmValue(const sAlarmIndex &index, sRelay::Alarm type);

protected:
    sTgUnit *getTgAlarmUnit(const sAlarmIndex &index);
    sAlarmUnit *getAlarmUnit(const sAlarmIndex &index);

private:
    sObjData *getObjData(const sAlarmIndex &index);
    sRelayUnit *getRelayUnit(const sAlarmIndex &index);
    sAlarmUnit *getAlarmUnit(const sAlarmIndex &index, sObjData *obj);
};

#endif // ALARM_OBJECT_H
