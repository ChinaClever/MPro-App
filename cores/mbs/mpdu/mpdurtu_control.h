#ifndef MPDURTU_CONTROL_H
#define MPDURTU_CONTROL_H

#include "mpdurtu_alarm.h"

class Mpdurtu_Control : public Mpdurtu_Alarm
{
public:
    explicit Mpdurtu_Control(QObject *parent = nullptr);

};

#endif // MPDURTU_CONTROL_H
