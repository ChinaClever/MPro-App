#ifndef APP_LDAP_H
#define APP_LDAP_H

#include "app_ntp.h"
struct sLdapCfg
{
    sLdapCfg() {}
    int en=0;
    QString url;
    QString user;
};


class App_Ldap : public App_Ntp
{
public:
    explicit App_Ldap(QObject *parent = nullptr);
    static sLdapCfg ldapCfg;
    bool ldap_work(const QString &pwd);
};

#endif // APP_LDAP_H
