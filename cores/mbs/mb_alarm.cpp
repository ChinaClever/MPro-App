/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_alarm.h"

Mb_Alarm::Mb_Alarm(QObject *parent) : Mb_Setting{parent}
{

}

void Mb_Alarm::mbAlarmUpdate()
{
    upLineCrAlarm();
    upOutputCrAlarm();
    upEnvAlarm();

    upCurMaxAlarm();
    upLineVolAlarm();
    upCurCrMaxAlarm();
    upLoopAlarm();
    upBreakerAlarm();
}

void Mb_Alarm::upBreakerAlarm()
{
    sDataItem index; vshort vs;
    index.type = DType::Loop;
    index.topic = DTopic::Relay;
    for(int i=0; i<LOOP_NUM; ++i) {
        index.id = i; vs << alarmValue(index, sRelay::OffALarm);
    }
    setCoils(MbReg_Alarms+368, vs);
}

void Mb_Alarm::upLineCrAlarm()
{
    sDataItem index; vshort vs;
    index.type = DType::Tg;
    index.topic = DTopic::Cur;

    vs << mDevData->alarm;
    vs << alarmValue(index, AlarmStatus::CrMax);
    vs << alarmValue(index, AlarmStatus::CrMin);
    vs << 0;

    index.type = DType::Line;
    for(int i=0; i<LINE_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::CrMax);
    }

    for(int i=0; i<LINE_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::CrMin);
    }

    setCoils(MbReg_Alarms, vs);
}

void Mb_Alarm::upEnvAlarm()
{
    sDataItem index; vshort vs;
    index.type = DType::Env;
    index.topic = DTopic::Tem;
    for(int i=0; i<SENOR_NUM; ++i) {
        index.id = i;
        vs << alarmValue(index, AlarmStatus::Max);
        vs << alarmValue(index, AlarmStatus::Min);
    } setCoils(MbReg_Alarms+154, vs); vs.clear();

    index.topic = DTopic::Hum;
    for(int i=0; i<SENOR_NUM; ++i) {
        index.id = i;
        vs << alarmValue(index, AlarmStatus::Max);
        vs << alarmValue(index, AlarmStatus::Min);
    } setCoils(MbReg_Alarms+170, vs); vs.clear();

    for(int i=0; i<SENOR_NUM; ++i) vs << mDevData->env.door[i];
    setCoils(MbReg_Alarms+186, vs); vs.clear();

    for(int i=0; i<SENOR_NUM; ++i) vs << mDevData->env.water[i];
    setCoils(MbReg_Alarms+202, vs); vs.clear();

    for(int i=0; i<SENOR_NUM; ++i) vs << mDevData->env.smoke[i];
    setCoils(MbReg_Alarms+210, vs); vs.clear();
}

void Mb_Alarm::upCurMaxAlarm()
{
    sDataItem index; vshort vs;
    index.type = DType::Tg;
    index.topic = DTopic::Cur;
    vs << alarmValue(index, AlarmStatus::Max);

    index.type = DType::Line;
    for(int i=0; i<LINE_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::Max);
    }

    index.type = DType::Output;
    for(int i=0; i<OUTPUT_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::Max);
    }

    setCoils(MbReg_Alarms+220, vs);
}

void Mb_Alarm::upLineVolAlarm()
{
    sDataItem index; vshort vs;
    index.type = DType::Tg;
    index.topic = DTopic::Vol;
    vs << alarmValue(index, AlarmStatus::Max);
    vs << alarmValue(index, AlarmStatus::Min);

    index.type = DType::Line;
    for(int i=0; i<LINE_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::Max);
    }

    for(int i=0; i<LINE_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::Min);
    }

    setCoils(MbReg_Alarms+272, vs);
}

void Mb_Alarm::upCurCrMaxAlarm()
{
    sDataItem index; vshort vs;
    index.type = DType::Tg;
    index.topic = DTopic::Cur;
    vs << alarmValue(index, AlarmStatus::CrMax);

    index.type = DType::Line;
    for(int i=0; i<LINE_NUM; ++i) {
        vs << alarmValue(index, AlarmStatus::CrMax);
    }

    index.type = DType::Output;
    for(int i=0; i<OUTPUT_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::CrMax);
    }

    setCoils(MbReg_Alarms+280, vs);
}

void Mb_Alarm::upLoopAlarm()
{
    sDataItem index; vshort vs;
    index.type = DType::Loop;
    index.topic = DTopic::Cur;
    for(int i=0; i<LOOP_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::Max);
    }

    for(int i=0; i<LOOP_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::CrMax);
    }

    for(int i=0; i<LOOP_NUM; ++i) {
        index.id = i; vs << alarmValue(index, AlarmStatus::Min);
    }

    setCoils(MbReg_Alarms+332, vs);
}

void Mb_Alarm::upOutputCrAlarm()
{
    sDataItem index; vshort vs;
    index.type = DType::Output;
    index.topic = DTopic::Cur;

    for(int i=0; i<OUTPUT_NUM; ++i) {
        index.id = i;
        vs << alarmValue(index, AlarmStatus::CrMax);
        vs << alarmValue(index, AlarmStatus::CrMin);
    }

    setCoils(MbReg_Alarms+10, vs);
}
