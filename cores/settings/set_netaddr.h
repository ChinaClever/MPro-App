#ifndef SET_NETADDR_H
#define SET_NETADDR_H
#include "set_integr.h"

class Set_NetAddr : public Set_Integr
{
public:
    Set_NetAddr();

    QVariant netAddrCfg(uchar fc, uchar sub);
    bool netAddrSet(sCfgItem &it, const QVariant &v);

private:
    bool isSetting = false;
    sNetInterface m_net;
};

#endif // SET_NETADDR_H
