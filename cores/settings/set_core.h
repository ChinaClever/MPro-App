#ifndef SET_CORE_H
#define SET_CORE_H
#include "set_output.h"

class Set_Core : public Set_Output
{
    Set_Core();
public:
    static Set_Core *bulid();
    void writeSettings();

    bool setUut(uchar addr, const sUutInfo &uut);
};

#endif // SET_CORE_H
