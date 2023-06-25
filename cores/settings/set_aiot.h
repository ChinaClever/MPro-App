#ifndef SET_AIOT_H
#define SET_AIOT_H
#include "set_netaddr.h"

class Set_Aiot : public Set_NetAddr
{
public:
    Set_Aiot();

    QVariant aiotCfg(int fc);
    bool aiotSet(int fc, const QVariant &v);


};

#endif // SET_AIOT_H
