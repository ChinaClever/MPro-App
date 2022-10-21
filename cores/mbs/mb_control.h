#ifndef MB_CONTROL_H
#define MB_CONTROL_H

#include "mb_alarm.h"

class Mb_Control : public Mb_Alarm
{
public:
    explicit Mb_Control(QObject *parent = nullptr);

protected:
    void mbCtrlUpdate();

private:
    void upRelayState();
};

#endif // MB_CONTROL_H
