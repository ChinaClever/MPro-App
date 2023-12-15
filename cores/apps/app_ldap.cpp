/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_ldap.h"

sLdapCfg App_Ldap::ldapCfg;
App_Ldap::App_Ldap(QObject *parent)
    : App_Ntp{parent}
{

}

bool App_Ldap::ldap_work(const QString &pwd)
{
    return false; // 此功能未启用
    QString fmd = "ldapbind %1 %2 %3"; sLdapCfg *cfg = &ldapCfg;
    QString str = fmd.arg(cfg->url, cfg->user, pwd);
    bool ret = false; str = cm::execute(str);
    if(str.contains("ok")) ret = true;
    return ret;
}
