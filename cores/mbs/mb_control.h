#ifndef MB_CONTROL_H
#define MB_CONTROL_H

#include "mb_alarm.h"

class Mb_Control : public Mb_Alarm
{
    Q_OBJECT
public:
    explicit Mb_Control(QObject *parent = nullptr);

signals:

};

#endif // MB_CONTROL_H
