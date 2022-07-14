#ifndef SET_INTEGR_H
#define SET_INTEGR_H
#include "set_output.h"

class Set_Integr : public Set_Output
{
public:
    Set_Integr();
protected:
    int modbusCfg(uchar fc);
    bool modbusSet(uchar fc, int value, uchar txType);
};

#endif // SET_INTEGR_H
