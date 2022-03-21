#include "mb_alarm.h"

Mb_Alarm::Mb_Alarm(QObject *parent) : Mb_Setting{parent}
{

}

void Mb_Alarm::mbAlarmUpdate()
{
    upLineCrAlarm();
    upOutputCrAlarm();

    upCurMaxAlarm();
    upLineVolAlarm();
    upCurMinAlarm();
    upLoopAlarm();
}

void Mb_Alarm::upLineCrAlarm()
{
    sAlarmIndex index; vshort vs;
    index.type = AlarmIndex::Tg;
    index.subtopic = AlarmIndex::Cur;

    vs << mDevData->alarm;
    vs << alarmValue(index, AlarmType::CrMax);
    vs << alarmValue(index, AlarmType::CrMin);
    vs << 0;

    index.type = AlarmIndex::Line;
    for(int i=0; i<LINE_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmType::CrMax);
    }

    for(int i=0; i<LINE_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmType::CrMin);
    }

    setCoils(MbReg_Alarms, vs);
}

void Mb_Alarm::upCurMaxAlarm()
{
    sAlarmIndex index; vshort vs;
    index.type = AlarmIndex::Tg;
    index.subtopic = AlarmIndex::Cur;
    vs << alarmValue(index, AlarmType::Max);

    index.type = AlarmIndex::Line;
    for(int i=0; i<LINE_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmType::Max);
    }

    index.type = AlarmIndex::Output;
    for(int i=0; i<OUTPUT_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmType::Max);
    }

    setCoils(MbReg_Alarms+220, vs);
}

void Mb_Alarm::upLineVolAlarm()
{
    sAlarmIndex index; vshort vs;
    index.type = AlarmIndex::Tg;
    index.subtopic = AlarmIndex::Vol;
    vs << alarmValue(index, AlarmType::Max);
    vs << alarmValue(index, AlarmType::Min);

    index.type = AlarmIndex::Line;
    for(int i=0; i<LINE_NUM; ++i) {
        index.id = i;
        vs << alarmValue(index, AlarmType::Max);
        vs << alarmValue(index, AlarmType::Min);
    }
    setCoils(MbReg_Alarms+272, vs);
}

void Mb_Alarm::upCurMinAlarm()
{
    sAlarmIndex index; vshort vs;
    index.type = AlarmIndex::Tg;
    index.subtopic = AlarmIndex::Cur;
    vs << alarmValue(index, AlarmType::Min);

    index.type = AlarmIndex::Output;
    for(int i=0; i<OUTPUT_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmType::Min);
    }

    setCoils(MbReg_Alarms+280, vs);
}

void Mb_Alarm::upLoopAlarm()
{
    sAlarmIndex index; vshort vs;
    index.type = AlarmIndex::Loop;
    index.subtopic = AlarmIndex::Cur;
    index.type = AlarmIndex::Loop;
    for(int i=0; i<2*LOOP_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmType::Max);
    }

    for(int i=0; i<2*LOOP_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmType::CrMax);
    }

    for(int i=0; i<2*LOOP_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmType::Min);
    }

    setCoils(MbReg_Alarms+332, vs);
}

void Mb_Alarm::upOutputCrAlarm()
{
    sAlarmIndex index; vshort vs;
    index.type = AlarmIndex::Output;
    index.subtopic = AlarmIndex::Cur;

    for(int i=0; i<OUTPUT_NUM; ++i) {
        index.id = i;
        vs << alarmValue(index, AlarmType::CrMax);
        vs << alarmValue(index, AlarmType::CrMin);
    }

    setCoils(MbReg_Alarms+10, vs);
}
