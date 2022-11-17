/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_radius.h"

sRadiusCfg App_Radius::radiusCfg;
App_Radius::App_Radius(QObject *parent)
    : App_Ntp{parent}
{

}


int App_Radius::radius_work(const QString &usr, const QString &pwd)
{
    QString fmd = "radiuscfg %1 %2 %3 %4"; sRadiusCfg *cfg = &radiusCfg;
    QString cmd = fmd.arg(cfg->host).arg(cfg->authPort).arg(cfg->acctPort).arg(cfg->key);
    cm::execute(cmd);  fmd = "radius %1 %2"; cmd = fmd.arg(usr, pwd);
    int ret = 0; QString res = cm::execute(cmd);
    if(res.contains("NORES")) ret = -1;
    else if(res.contains("ERR")) ret = 0;
    else ret = 1;
    return ret;
}
