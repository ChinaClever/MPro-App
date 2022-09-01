#ifndef DATA_DUAL_H
#define DATA_DUAL_H
#include "data_env.h"

class Data_Dual : public Data_Env
{
public:
    Data_Dual();

protected:
    void dualWork();

private:
    void dualData(int id=1);
    void dualTiming();
    int setDualSize();
    void disDualAlarm();
};

#endif // DATA_DUAL_H
