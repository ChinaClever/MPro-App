/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_service.h"
#include "app_core.h"
#include "sercret_core.h"

Set_Service::Set_Service()
{

}

QVariant Set_Service::logCfg(int fc)
{
    sLogCfg *cfg = &Log_Core::cfg;
    QVariant ret; switch (fc) {
    case 1: ret = cfg->hdaEn; break;
    case 2: ret = cfg->eleTime; break;
    case 3: ret = cfg->hdaTime; break;
    case 4: ret = cfg->logCnt; break;
    case 5: ret = cfg->eventCnt; break;
    case 6: ret = cfg->hdaCnt; break;
    default: cout << fc; break;
    }
    return ret;
}

bool Set_Service::logSet(int fc, const QVariant &v)
{   
    sLogCfg *cfg = &Log_Core::cfg;
    QString prefix = "log"; QString key;
    bool ret = true; switch (fc) {
    case 1: key = "hdaEn"; cfg->hdaEn = v.toInt(); break;
    case 2: key = "eleTime"; cfg->eleTime = v.toInt(); break;
    case 3: key = "hdaTime";  cfg->hdaTime = v.toInt(); break;
    case 4: key = "logCnt";  cfg->logCnt = v.toInt();  break;
    case 5: key = "eventCnt";  cfg->eventCnt = v.toInt();  break;
    case 6: key = "hdaCnt";  cfg->hdaCnt = v.toInt();  break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}

QVariant Set_Service::syslogCfg(int fc)
{
    QVariant ret;
    sSysLogCfg *cfg = &Log_Sys::sysLogCfg;
    switch (fc) {
    case 1: ret = cfg->en; break;
    case 2: ret = cfg->host; break;
    case 3: ret = cfg->port; break;
    default: cout << fc; break;
    }

    return ret;
}

bool Set_Service::syslogSet(int fc, const QVariant &v)
{
    bool ret = true;
    sSysLogCfg *cfg = &Log_Sys::sysLogCfg;
    QString prefix = "syslog"; QString key;

    switch (fc) {
    case 1: key = "en"; cfg->en = v.toInt(); break;
    case 3: key = "port";  cfg->port = v.toInt(); break;
    case 2: key = "host";  cfg->host = v.toString(); break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}

QVariant Set_Service::smtpCfg(int fc, int id)
{
    QVariant ret; if(id) id -= 1;
    sSmtpCfg *it = &App_Smtp::smtpCfg;
    switch (fc) {
    case 1: ret = it->en; break;
    case 2: ret = it->host; break;
    case 3: ret = it->from; break;
    case 4: ret = it->pwd; break;
    case 5: ret = it->to[id]; break;
    case 6: ret = it->port; break;
    case 7: ret = it->ct; break;
    case 8: ret = it->lastErr; break;
    case 11: ret = App_Core::bulid()->smtp_testMail(); break;
    default: cout << fc; break;
    }
    return ret;
}

bool Set_Service::smtpSet(int fc, int id, const QVariant &v)
{
    if(id) id -= 1; // cout << id;
    sSmtpCfg *it = &App_Smtp::smtpCfg;
    QString prefix = "smtp"; QString key;
    bool ret = true; switch (fc) {
    case 1: key = "en"; it->en = v.toInt(); break;
    case 2: key = "host";  it->host = v.toString(); break;
    case 3: key = "from";  it->from = v.toString();  break;
    case 4: key = "pwd";  it->pwd =v.toString(); break;
    case 5: key = "to_"+QString::number(id); it->to[id] =v.toString();  break;
    case 6: key = "port";  it->port =v.toInt();  break;
    case 7: key = "ct";  it->ct =v.toInt();  break;
    case 11: ret = App_Core::bulid()->smtp_testMail(); break;
    default: ret = false; cout << fc << v; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}


QVariant Set_Service::scriptCfg(int fc, int id)
{
    QVariant ret; QString str;
    sScriptCfg *cfg = &App_Script::scriptCfg;
    switch (fc) {
    case 0: ret = cfg->type[id]; break;
    case 1: ret = cfg->startup[id]; break;
    case 2: ret = cfg->cmd[id]; break;
    case 3: case 5: ret = ""; break;
    case 4: foreach (auto it, cfg->result[id]) str.append(it + "\n"); ret = str; break; //.remove(QRegExp("\n{2,}"))
    default: cout << fc; break;
    }

    return ret;
}

bool Set_Service::scriptSet(int fc, int id, const QVariant &v)
{
    QString prefix = "script"; QString key; bool ret = true;
    sScriptCfg *cfg = &App_Script::scriptCfg;

    switch (fc) {
    case 0: key = "type_%1"; cfg->type[id] = v.toInt(); break;
    case 1: key = "startup_%1"; cfg->startup[id] = v.toInt(); break;
    case 2: key = "cmd_%1"; cfg->cmd[id] = v.toString(); break;
    case 3: App_Core::bulid()->script_execute(id); break;
    case 4: cfg->result[id].clear(); break;
    case 5: App_Core::bulid()->script_kill(id); break;
    default: cout << fc; ret = false; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key.arg(id), v, prefix);
    }
    return ret;
}

QVariant Set_Service::sshCfg(int fc)
{
    QVariant ret;
    sSshCfg *cfg = &App_Ssh::sshCfg;
    switch (fc) {
    case 1: ret = cfg->ssh_en; break;
    case 2: ret = cfg->telnet_en; break;
    case 3: ret = cfg->usr; break;
    case 4: ret = cfg->pwd; break;
    default: cout << fc; break;
    }

    return ret;
}

bool Set_Service::sshSet(int fc, const QVariant &v)
{
    sSshCfg *cfg = &App_Ssh::sshCfg;
    App_Core *obj = App_Core::bulid();
    QString prefix = "ssh"; QString key; bool ret = true;
    ushort strong_pwd = cm::dataPacket()->web.strong_pwd;

    switch (fc) {
    case 1: key = "ssh_en"; cfg->ssh_en = v.toInt(); break;
    case 2: key = "telnet_en";  cfg->telnet_en = v.toInt(); break;
    case 3: key = "usr";  cfg->usr = v.toString();  break;
    case 4: key = "pwd";  cfg->pwd = v.toString();  break;
    case 5: ret = obj->ssh_save(); break;
    default: ret = false; cout << fc; break;
    } if(3==fc && cfg->usr.isEmpty()) obj->ssh_delUser();

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid(); if(fc == 4 && strong_pwd) {
            QByteArray str = Sercret_Core::bulid()->rsa_encode(v.toByteArray());
            cfg->writeCfg(key, str, prefix);
        } else cfg->writeCfg(key, v, prefix);
    }

    return ret;
}

QVariant Set_Service::ntpCfg(int fc)
{
    QVariant ret;
    sNtpCfg *it = &App_Ntp::ntpCfg;
    App_Core *obj = App_Core::bulid();
    switch (fc) {
    case 1: ret = obj->ntp_time(); break;
    case 2: ret = it->udp_en; break;
    case 3: ret = it->ntp_host; break;
    case 4: ret = it->time_zone; break;
    case 5: ret = obj->ntpdate(); break;
    case 6: ret = cm::pingNet(it->ntp_host); break;
    default: cout << fc; break;
    }
    return ret;
}

bool Set_Service::ntpSet(int fc, const QVariant &v)
{
    bool ret = true;
    sNtpCfg *it = &App_Ntp::ntpCfg;
    App_Core *obj = App_Core::bulid();
    QString prefix = "ntp"; QString key;

    switch (fc) {
    case 1: ret = obj->ntp_time(v.toString()); break;
    case 2: key = "udp_en";  obj->ntp_udpEn(v.toInt()); break;
    case 3: key = "ntp_host";  it->ntp_host = v.toString();  break;
    case 4: key = "time_zone";  obj->ntp_timeZone(v.toString());  break;
    case 5: obj->ntpdate(); break; case 6: ret = cm::pingNet(it->ntp_host); break;
    default: ret = false; cout << fc; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}

int Set_Service::webCfg(int fc)
{    
    sWebCfg *it = &cm::dataPacket()->web;
    int ret = 0; switch (fc) {
    case 1: ret = it->http_en; break;
    case 2: ret = it->http_port; break;
    case 3: ret = it->http_redirect; break;
    case 4: ret = it->https_en; break;
    case 5: ret = it->https_port; break;
    case 6: ret = it->strong_pwd; break;
    case 7: ret = it->idle_timeout; break;
    case 8: ret = it->multi_users; break;
    default: cout << fc; break;
    } //cout << fc << ret;

    return ret;
}

bool Set_Service::webSet(int fc, const QVariant &v)
{   
    sWebCfg *it = &cm::dataPacket()->web;
    QString prefix = "web"; QString key;
    bool ret = true; switch (fc) {
    case 1: key = "http_en";  it->http_en = v.toInt(); break;
    case 2: key = "http_port";  it->http_port = v.toInt(); break;
    case 3: key = "http_redirect";  it->http_redirect = v.toInt();  break;
    case 4: key = "https_en";  it->https_en =v.toInt();  break;
    case 5: key = "https_port";  it->https_port = v.toInt(); break;
    case 6: key = "strong_pwd";  it->strong_pwd = v.toInt(); break;
    case 7: key = "idle_timeout";  it->idle_timeout = v.toInt(); break;
    case 8: key = "multi_users";  it->multi_users = v.toInt(); break;
    default: ret = false; cout<< fc; break;
    } //cout << key << fc << v;

    if(3 == fc && it->http_redirect && !it->https_en) {
        it->http_redirect = 0; return false;
    } else if(4 == fc && it->http_redirect && !it->https_en) {
        it->https_en = 1; return false;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
        if(!it->http_en && !it->https_en) {
            key = "http_en"; it->http_en =1;
            cfg->writeCfg(key, 1, prefix);
        }
    }
    return ret;
}

QVariant Set_Service::raduisCfg(int fc)
{
    sRadiusCfg *cfg = &App_Radius::radiusCfg;
    QVariant ret; switch (fc) {
    case 1: ret = cfg->en; break;
    case 2: ret = cfg->local; break;
    case 3: ret = cfg->host; break;
    case 4: ret = cfg->key; break;
    case 5: ret = cfg->authPort; break;
    case 6: ret = cfg->acctPort; break;
    default: cout << fc; break;
    }

    return ret;
}

bool Set_Service::raduisSet(int fc, const QVariant &v)
{
    QString prefix = "raduis"; QString key;
    sRadiusCfg *cfg = &App_Radius::radiusCfg;

    bool ret = true; switch (fc) {
    case 1: key = "en"; cfg->en = v.toInt(); break;
    case 2: key = "local";  cfg->local = v.toInt(); break;
    case 3: key = "host";  cfg->host = v.toString();  break;
    case 4: key = "key";  cfg->key = v.toString();  break;
    case 5: key = "authPort";  cfg->authPort = v.toInt();  break;
    case 6: key = "acctPort";  cfg->acctPort = v.toInt();  break;
    default: ret = false; cout << fc;  break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}

QVariant Set_Service::ldapCfg(int fc)
{
    sLdapCfg *cfg = &App_Ldap::ldapCfg;
    QVariant ret; switch (fc) {
    case 1: ret = cfg->en; break;
    case 2: ret = cfg->url; break;
    case 3: ret = cfg->user; break;
    default: cout << fc; break;
    }

    return ret;
}

bool Set_Service::ldapSet(int fc, const QVariant &v)
{
    QString prefix = "openldap"; QString key;
    sLdapCfg *cfg = &App_Ldap::ldapCfg;

    bool ret = true; switch (fc) {
    case 1: key = "en"; cfg->en = v.toInt(); break;
    case 2: key = "url";  cfg->url = v.toString(); break;
    case 3: key = "user";  cfg->user = v.toString();  break;
    default: ret = false; cout << fc;  break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}
