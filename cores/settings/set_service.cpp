/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_service.h"
#include "web_server/web_core.h"
#include "app_core.h"

Set_Service::Set_Service()
{

}

QVariant Set_Service::smtpCfg(int fc)
{
    QVariant ret;
    sSmtpCfg *it = &App_Smtp::smtpCfg;
    switch (fc) {
    case 1: ret = it->en; break;
    case 2: ret = it->host; break;
    case 3: ret = it->from; break;
    case 4: ret = it->pwd; break;
    case 5: ret = it->to; break;
    case 6: ret = it->port; break;
    case 7: ret = it->ct; break;
    case 8: ret = it->lastErr; break;
    default: cout << fc; break;
    }
    return ret;
}

bool Set_Service::smtpSet(int fc, const QVariant &v)
{
    bool ret = true;
    sSmtpCfg *it = &App_Smtp::smtpCfg;
    QString prefix = "smtp"; QString key;

    switch (fc) {
    case 1: key = "en"; it->en = v.toInt(); break;
    case 2: key = "host";  it->host = v.toString(); break;
    case 3: key = "from";  it->from = v.toString();  break;
    case 4: key = "pwd";  it->pwd =v.toString();  break;
    case 5: key = "to";  it->to =v.toString();  break;
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

QVariant Set_Service::ntpCfg(int fc)
{
    QVariant ret;
    sNtpCfg *it = &App_Ntp::ntpCfg;
    App_Core *obj = App_Core::bulid();
    switch (fc) {
    case 1: ret = obj->ntp_time(); break;
    case 2: ret = it->udp_en; break;
    case 3: ret = it->ntp_host; break;
    case 4: ret = obj->ntp_timeZone(); break;
    case 5: ret = obj->ntpdate(); break;
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
    case 4: key = "time_zone";  it->time_zone =v.toString();  break;
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
    sWebCfg *it = &Web_Core::cfg;
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
    sWebCfg *it = &Web_Core::cfg;
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
