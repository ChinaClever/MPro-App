#ifndef DATA_OUTLET_H
#define DATA_OUTLET_H
#include "data_relay.h"
#include "log_core.h"

class Data_Outlet : public Data_Relay
{
public:
    Data_Outlet();

protected:
    void outletWork();

private:
    void outletNums();
    void outletTiming();
    void outletOverrunOff();

};

#endif // DATA_OUTLET_H
