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

    QVariant smtpCfg(int fc, int id);
    bool smtpSet(int fc, int id, const QVariant &v);

    QVariant sshCfg(int fc);
    bool sshSet(int fc, const QVariant &v);

    QVariant syslogCfg(int fc);
    bool syslogSet(int fc, const QVariant &v);

    QVariant logCfg(int fc);
    bool logSet(int fc, const QVariant &v);

    QVariant raduisCfg(int fc);
    bool raduisSet(int fc, const QVariant &v);

    QVariant ldapCfg(int fc);
    bool ldapSet(int fc, const QVariant &v);
};

#endif // SET_SERVICE_H
