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
    void dualWorkdown(int type);
    void dualTgWork();


    int setDualSize(int id, int type);
    void dualData(int id, int type);
    void dualTiming(int id);
    void disDualAlarm();
};

#endif // DATA_DUAL_H
