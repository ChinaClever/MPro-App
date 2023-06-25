/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "aiot_outlet.h"

Aiot_Outlet::Aiot_Outlet()
{

}

void Aiot_Outlet::outletPost()
{
     outletEles();
     outletPows();
     outletRelays();
}

void Aiot_Outlet::outletRelays()
{
    double unit = 1;
    QString key = "outletSwitch";
    QString module = "RelayStatus";
    int size = mDevData->output.relay.size;
    uint *ptr = mDevData->output.relay.sw;
    appendArrayProperty(ptr, size, unit, key, module);
}

void Aiot_Outlet::outletPows()
{
    double unit = COM_RATE_POW;
    QString key = "outletPow";
    QString module = "OutletPows";
    int size = mDevData->output.pow.size;
    uint *ptr = mDevData->output.pow.value;
    appendArrayProperty(ptr, size, unit, key, module);
}

void Aiot_Outlet::outletEles()
{
    double unit = COM_RATE_ELE;
    QString key = "outletEle";
    QString module = "OutletEles";
    int size = mDevData->output.size;
    uint *ptr = mDevData->output.ele;
    appendArrayProperty(ptr, size, unit, key, module);
}
