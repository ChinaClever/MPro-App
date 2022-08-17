#ifndef DATA_LOOP_H
#define DATA_LOOP_H
#include "data_outlet.h"

class Data_Loop : public Data_Outlet
{
public:
    Data_Loop();

protected:
    void loopWork();

private:
    void loopNum();
};

#endif // DATA_LOOP_H
