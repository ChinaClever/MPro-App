#ifndef APP_RADIUS_H
#define APP_RADIUS_H
#include "app_ldap.h"
struct sRadiusCfg
{
    sRadiusCfg() {}
    int en, local;
    QString host;
    QString key;
    int authPort; // 认证端口
    int acctPort; // 账号端口
};

class App_Radius : public App_Ldap
{
public:
    App_Radius(QObject *parent = nullptr);
    static sRadiusCfg radiusCfg;
    int radius_work(const QString &usr, const QString &pwd);
};

#endif // APP_RADIUS_H
