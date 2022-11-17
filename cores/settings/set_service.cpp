/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_service.h"
#include "app_core.h"

Set_Service::Set_Service()
{

}

QVariant Set_Service::logCfg(int fc)
{
    sLogCfg *cfg = &Log_Core::cfg;
    QVariant ret; switch (fc) {
    case 1: ret = cfg->eleTime; break;
    case 2: ret = cfg->hdaTime; break;
    case 3: ret = cfg->logCnt; break;
    case 4: ret = cfg->hdaCnt; break;
    case 5: ret = cfg->eventCnt; break;
    default: cout << fc; break;
    }

    return ret;
}


bool Set_Service::logSet(int fc, const QVariant &v)
{
    bool ret = true;
    sLogCfg *cfg = &Log_Core::cfg;
    QString prefix = "log"; QString key;

    switch (fc) {
    case 1: key = "eleTime"; cfg->eleTime = v.toInt(); break;
    case 2: key = "hdaTime";  cfg->hdaTime = v.toInt(); break;
    case 3: key = "logCnt";  cfg->logCnt = v.toInt();  break;
    case 4: key = "hdaCnt";  cfg->hdaCnt = v.toInt();  break;
    case 5: key = "eventCnt";  cfg->eventCnt = v.toInt();  break;
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
    case 2: key = "port";  cfg->port = v.toInt(); break;
    case 3: key = "host";  cfg->host = v.toString();  break;
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
    QVariant ret;
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
    default: cout << fc; break;
    }
    return ret;
}

bool Set_Service::smtpSet(int fc, int id, const QVariant &v)
{
    bool ret = true;
    sSmtpCfg *it = &App_Smtp::smtpCfg;
    QString prefix = "smtp"; QString key;

    switch (fc) {
    case 1: key = "en"; it->en = v.toInt(); break;
    case 2: key = "host";  it->host = v.toString(); break;
    case 3: key = "from";  it->from = v.toString();  break;
    case 4: key = "pwd";  it->pwd =v.toString();  break;
    case 5: key = "to_"+QString::number(id); it->to[id] =v.toString();  break;
    case 6: key = "port";  it->port =v.toInt();  break;
    case 7: key = "ct";  it->ct =v.toInt();  break;
    default: ret = false; cout << fc << v; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
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
    bool ret = true;
    sSshCfg *cfg = &App_Ssh::sshCfg;
    //App_Core *obj = App_Core::bulid();
    QString prefix = "ssh"; QString key;

    switch (fc) {
    case 1: key = "ssh_en"; cfg->ssh_en = v.toInt(); break;
    case 2: key = "telnet_en";  cfg->telnet_en = v.toInt(); break;
    case 3: key = "usr";  cfg->usr = v.toString();  break;
    case 4: key = "pwd";  cfg->pwd = v.toString();  break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
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
    case 2: key = "udp_en";  obj->ntp_udpEn(v.toBool()); break;
    case 3: key = "ntp_host";  it->ntp_host = v.toString();  break;
    case 4: key = "time_zone";  obj->ntp_timeZone(v.toString());  break;
    case 5: obj->ntpdate(); break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}

int Set_Service::webCfg(int fc)
{
    int ret = 0;
    sWebCfg *it = &cm::dataPacket()->web;
    switch (fc) {
    case 1: ret = it->http_en; break;
    case 2: ret = it->http_port; break;
    case 3: ret = it->http_redirect; break;
    case 4: ret = it->https_en; break;
    case 5: ret = it->https_port; break;
    default: cout << fc; break;
    }

    return ret;
}

bool Set_Service::webSet(int fc, const QVariant &v)
{
    bool ret = true;
    sWebCfg *it = &cm::dataPacket()->web;
    QString prefix = "web"; QString key;

    switch (fc) {
    case 1: key = "http_en";  it->http_en = v.toInt(); break;
    case 2: key = "http_port";  it->http_port = v.toInt(); break;
    case 3: key = "http_redirect";  it->http_port = v.toInt();  break;
    case 4: key = "https_en";  it->https_en =v.toInt();  break;
    case 5: key = "https_port";  it->https_port = v.toInt(); break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
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
