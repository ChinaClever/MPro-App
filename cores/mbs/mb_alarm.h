#ifndef MB_ALARM_H
#define MB_ALARM_H

#include "mb_setting.h"

class Mb_Alarm : public Mb_Setting
{
public:
    explicit Mb_Alarm(QObject *parent = nullptr);

protected:
    void mbAlarmUpdate();

private:
    void upLineCrAlarm();
    void upOutputCrAlarm();
    void upEnvAlarm();

    void upCurMaxAlarm();
    void upLineVolAlarm();
    void upCurCrMaxAlarm();
    void upLoopAlarm();
    void upBreakerAlarm();
};

#endif // MB_ALARM_H
