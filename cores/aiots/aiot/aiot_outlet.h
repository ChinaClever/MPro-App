#ifndef AIOT_OUTLET_H
#define AIOT_OUTLET_H
#include "aiot_loop.h"

class Aiot_Outlet : public Aiot_Loop
{
public:
    Aiot_Outlet();
    void outletPost();

private:
    void outletEles();
    void outletPows();
    void outletRelays();
};

#endif // AIOT_OUTLET_H
