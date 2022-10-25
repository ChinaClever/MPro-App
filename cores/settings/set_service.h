#ifndef SET_SERVICE_H
#define SET_SERVICE_H
#include "set_netaddr.h"

class Set_Service : public Set_NetAddr
{
public:
    Set_Service();

    int webCfg(int fc);
    bool webSet(int fc, const QVariant &v);

    QVariant ntpCfg(int fc);
    bool ntpSet(int fc, const QVariant &v);

    QVariant smtpCfg(int fc);
    bool smtpSet(int fc, const QVariant &v);

    QVariant sshCfg(int fc);
    bool sshSet(int fc, const QVariant &v);

};

#endif // SET_SERVICE_H
