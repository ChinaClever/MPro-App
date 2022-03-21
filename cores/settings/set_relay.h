#ifndef SET_RELAY_H
#define SET_RELAY_H

#include "set_alarm.h"

class Set_Relay : public Set_Alarm
{
public:
    explicit Set_Relay();
    bool outputRelayCtrl(int addr, int id, uchar on);
    bool outputRelaySet(int addr, int id, uchar mode, uchar delay);
};

#endif // SET_RELAY_H
