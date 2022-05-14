#ifndef SET_CORE_H
#define SET_CORE_H
#include "set_relay.h"

class Set_Core : public Set_Relay
{
    Set_Core();
public:
    static Set_Core *bulid();
    void writeSettings();
};

#endif // SET_CORE_H
