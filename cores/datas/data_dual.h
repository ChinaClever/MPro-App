#ifndef DATA_DUAL_H
#define DATA_DUAL_H
#include "data_group.h"

class Data_Dual : public Data_Group
{
public:
    Data_Dual();

protected:
    void dualWork();

private:
    void dualData(int id=1);
    int setDualSize();
    void disDualAlarm();
};

#endif // DATA_DUAL_H
