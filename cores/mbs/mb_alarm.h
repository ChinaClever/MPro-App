#ifndef MB_ALARM_H
#define MB_ALARM_H

#include "mb_setting.h"

class Mb_Alarm : public Mb_Setting
{
    Q_OBJECT
public:
    explicit Mb_Alarm(QObject *parent = nullptr);

signals:
protected:
    void mbAlarmUpdate();

private:
    void upLineCrAlarm();
    void upOutputCrAlarm();

    void upCurMaxAlarm();
    void upLineVolAlarm();
    void upCurMinAlarm();
    void upLoopAlarm();

};

#endif // MB_ALARM_H
