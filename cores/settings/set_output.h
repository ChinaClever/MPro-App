#ifndef SET_OUTPUT_H
#define SET_OUTPUT_H

#include "set_alarm.h"

class Set_Output : public Set_Alarm
{
public:
    explicit Set_Output();
    bool outputRelayCtrl(int addr, int id, uchar on);
    bool outputDelaySet(int addr, int id, uchar delay);
    bool outputSwModeSet(int addr, int id, uchar mode);
    bool outputNameSet(int addr, int id, const QString &name);
};

#endif // SET_OUTPUT_H
