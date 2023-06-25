#ifndef AIOT_DEVCFG_H
#define AIOT_DEVCFG_H
#include "aiot_outlet.h"

class Aiot_DevCfg : public Aiot_Outlet
{
public:
    Aiot_DevCfg();

    void basePost();
    void devCfgPost();
};

#endif // AIOT_DEVCFG_H
