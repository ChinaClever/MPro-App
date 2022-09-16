/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "web_server/web_core.h"
#include "cfg_readparam.h"
#include "rpc_service.h"
#include "integr_core.h"
#include "sercret_core.h"
#include "mqtt_client.h"
#include "agent_core.h"
#include "app_core.h"
#include "mb_core.h"

Cfg_ReadParam::Cfg_ReadParam(QObject *parent)
    : Cfg_AlarmInitial{parent}
{
    mCfg = Cfg_Com::bulid();
}

void Cfg_ReadParam::readCfgParams()
{
    ntp();
    web();
    rpc();
    push();
    snmp();
    smtp();
    mqtt();
    login();
    modbus();
    sercret();
    dualName();
    groupName();
    outputName();
}


void Cfg_ReadParam::smtp()
{
    sSmtpCfg *cfg = &App_Smtp::smtpCfg;
    QString prefix = "smtp";  QString key;
    QString *str = nullptr; int *ptr = nullptr;
    for(int i=1; i<8; ++i) {
        switch (i) {
        case 1: key = "en"; ptr = &cfg->en; break;
        case 2: key = "host"; str = &cfg->host; break;
        case 3: key = "from"; str = &cfg->from; break;
        case 4: key = "pwd"; str = &cfg->pwd; break;
        case 5: key = "to"; str = &cfg->to; break;
        case 6: key = "port"; ptr = &cfg->port; break;
        case 7: key = "ct"; ptr = &cfg->ct; break;
        default: ptr = nullptr; str = nullptr; break;
        }

        if(str) *str = mCfg->readCfg(key, "", prefix).toString();
        else if(ptr) *ptr = mCfg->readCfg(key, 0, prefix).toInt();
    }
}

void Cfg_ReadParam::snmp()
{
    sAgentCfg *cfg = &(Agent_Core::snmpCfg);
    QString prefix = "snmp";  QString key;
    QString *ptr = nullptr;
    for(int i=1; i<7; ++i) {
        switch (i) {
        case 1: key = "trap1"; ptr = &cfg->trap1; break;
        case 2: key = "trap2"; ptr = &cfg->trap2; break;
        case 3: key = "enV3"; break;
        case 4: key = "usr"; ptr = &cfg->usr; break;
        case 5: key = "pwd"; ptr = &cfg->pwd; break;
        case 6: key = "key"; ptr = &cfg->key; break;
        default: ptr = nullptr; break;
        }

        if(ptr) *ptr = mCfg->readCfg(key, "", prefix).toString();
        else cfg->enV3 = mCfg->readCfg(key, "", prefix).toInt();
    }
}

void Cfg_ReadParam::web()
{
    int *ptr = nullptr; int value = 0;
    QString prefix = "web"; QString key;
    sWebCfg *cfg = &Web_Core::cfg;

    for(int i=1; i<6; ++i) {
        switch (i) {
        case 1: key = "http_en"; ptr = &cfg->http_en; value = 1; break;
        case 2: key = "http_port"; ptr = &cfg->http_port; value = 80; break;
        case 3: key = "http_redirect"; ptr = &cfg->http_redirect; value = 0; break;
        case 4: key = "https_en"; ptr = &cfg->https_en; value = 1; break;
        case 5: key = "https_port"; ptr = &cfg->https_port;  value = 443; break;
        default: key.clear(); break;
        }

        if(key.size() && ptr) {
            *ptr = mCfg->readCfg(key, value, prefix).toInt();
        }
    }
}

void Cfg_ReadParam::ntp()
{
    sNtpCfg *it = &App_Ntp::ntpCfg;
    QString prefix = "ntp"; QString key;

    for(int i=1; i<5; ++i)  {
        switch (i) {
        case 2: key = "udp_en";  it->udp_en = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 3: key = "ntp_host";  it->ntp_host = mCfg->readCfg(key, "", prefix).toString();  break;
        case 4: key = "time_zone";  it->time_zone =mCfg->readCfg(key, "", prefix).toString();  break;
        }
    }
}

void Cfg_ReadParam::mqtt()
{
    QString prefix = "mqtt"; QString key;
    sMqttCfg *cfg = &(Mqtt_Client::cfg);
    for(int i=1; i<10; ++i) {
        switch (i) {
        case 1: key = "type"; cfg->type = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 2: key = "url"; cfg->url = mCfg->readCfg(key, "", prefix).toString(); break;
        case 3: key = "port"; cfg->port = mCfg->readCfg(key, 1883, prefix).toInt(); break;
        case 4: key = "path"; cfg->path = mCfg->readCfg(key, "/mqtt", prefix).toString(); break;
        case 5: key = "clientId"; cfg->clientId = mCfg->readCfg(key, "", prefix).toString(); break;
        case 6: key = "usr"; cfg->usr = mCfg->readCfg(key, "", prefix).toByteArray(); break;
        case 7: key = "pwd"; cfg->pwd = mCfg->readCfg(key, "", prefix).toByteArray(); break;
        case 8: key = "keepAlive"; cfg->keepAlive = mCfg->readCfg(key, 60, prefix).toInt();break;
        case 9: key = "qos"; cfg->qos = mCfg->readCfg(key, 0, prefix).toInt(); break;
        default: key.clear(); break;
        }
    }
}

void Cfg_ReadParam::login()
{
    QString prefix = "login";
    QString key; char *ptr=nullptr;
    sDevLogin *it = &(cm::dataPacket()->login[0]);

    for(int i=1; i<4; ++i) {
        switch (i) {
        case 1: key = "user";  ptr = it->user; break;
        case 2: key = "pwd";  ptr = it->pwd; break;
        case 3: key = "token";  ptr = it->token; break;
        case 4: key = "permit";  ptr = it->permit; break;
        }
        QString res = mCfg->readCfg(key, "", prefix).toString();
        qstrcpy(ptr, res.toLatin1().data());
    }
}

void Cfg_ReadParam::modbus()
{
    int *ptr = nullptr; int value = 0;
    QString prefix = "modbus"; QString key;
    sModbusSetting *cfg = &(Mb_Core::modbusCfg);
    for(int i=0; i<13; ++i) {
        switch (i) {
        case 1: key = "enRtu";  ptr = &cfg->enRtu; value = 0; break;
        case 2: key = "addr";  ptr = &cfg->addr; value = 1; break;
        case 3: key = "baud";  ptr = &cfg->baud; value = 9600; break;
        case 4: key = "parity";  ptr = &cfg->parity; value = 0; break;
        case 5: key = "dataBits";  ptr = &cfg->dataBits; value = 8;  break;
        case 6: key = "stopBits";  ptr = &cfg->stopBits; value = 1; break;
        case 11: key = "enTcp";  ptr = &cfg->enTcp; value = 0;  break;
        case 12: key = "port";  ptr = &cfg->port; value = 502; break;
        default: key.clear(); break;
        }
        if(key.size() && ptr) *ptr = mCfg->readCfg(key, value, prefix).toInt();
    }
}

void Cfg_ReadParam::rpc()
{
    int *ptr = nullptr; int value = 0;
    QString prefix = "rpc"; QString key;
    sRpcCfg *cfg = &(Rpc_Service::rpcCfg);

    for(int i=1; i<6; ++i) {
        switch (i) {
        case 1: key = "jsonRpcEn";  ptr = &cfg->json.en; value = 0; break;
        case 2: key = "jsonRpcPort";  ptr = &cfg->json.port; value = 6002; break;
        case 4: key = "xmlRpcEn";  ptr = &cfg->xml.en; value = 0; break;
        case 5: key = "xmlRpcPort";  ptr = &cfg->xml.port; value = 6082; break;
        default: key.clear(); break;
        }
        if(key.size() && ptr) *ptr = mCfg->readCfg(key, value, prefix).toInt();
    }
}

void Cfg_ReadParam::sercret()
{
    QByteArray *arr = nullptr;
    QString prefix = "sercret"; QString key;
    Sercret_Core *it = Sercret_Core::bulid();

    for(int i=1; i<36; ++i) {
        switch (i) {
        case 1: key = "en"; it->cfg.type = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 11: key = "aes_mode"; it->aes_setMode(mCfg->readCfg(key, 1, prefix).toInt()); break;
        case 12: key = "aes_padding"; it->aes_setPadding(mCfg->readCfg(key, 2, prefix).toInt()); break;
        case 13: key = "aes_level"; it->aes_setLevel(mCfg->readCfg(key, 2, prefix).toInt()); break;
        case 14: key = "aes_key"; arr = &it->aesCfg.key; break;
        case 15: key = "aes_iv"; arr = &it->aesCfg.iv; break;
        case 33: key = "sm4_key"; arr = &it->sm4Cfg.key;break;
        case 34: key = "sm4_iv"; arr = &it->sm4Cfg.iv; break;
        default: key.clear(); break;
        }

        if(key.size()) {
            if(arr) *arr = mCfg->readCfg(key, "", prefix).toByteArray();
            arr = nullptr;
        }
    }
}

void Cfg_ReadParam::push()
{
    QString *str = nullptr;
    int *ptr = nullptr; int value = 0;
    QString prefix = "push"; QString key;
    sPushCfg *cfg = &Integr_Core::pushCfg;

    for(int i=1; i<16; ++i) {
        switch (i) {
        case 1: key = "udpEn"; ptr = &cfg->udp[0].en; value = 0; break;
        case 2: key = "ddpHost"; str = &cfg->udp[0].host; break;
        case 3: key = "udpPort"; ptr = &cfg->udp[0].port; value = 1124; break;
        case 4: key = "udp2En"; ptr = &cfg->udp[1].en; value = 0; break;
        case 5: key = "ddp2Host"; str = &cfg->udp[1].host; break;
        case 6: key = "udp2Port"; ptr = &cfg->udp[1].port; value = 1125; break;
        case 7: key = "recvEn"; ptr = &cfg->recvEn; value = 0; break;
        case 8: key = "recvPort"; ptr = &cfg->recvPort; value = 3096; break;
        case 9: key = "sec"; ptr = &cfg->sec; value = 5; break;
        case 10: key = "dc";  ptr = &cfg->dataContent; value = 0; break;
        case 11: key = "httpEn"; ptr = &cfg->http.en; value = 0; break;
        case 12: key = "httpUrl"; str = &cfg->http.url; break;
        case 13: key = "httpTimeout"; ptr = &cfg->http.timeout; value = 1;break;
        case 14: key = "enServ"; ptr = &cfg->http.enServ; value = 0; break;
        case 15: key = "httpPort"; ptr = &cfg->http.port; value = 3166;break;
        default: key.clear(); break;
        }

        if(key.size()) {
            if(ptr) *ptr = mCfg->readCfg(key, value, prefix).toInt();
            else *str = mCfg->readCfg(key, "", prefix).toString();
            ptr = nullptr;
        }
    }
}

void Cfg_ReadParam::dualName()
{
    QString prefix = "DualName";
    sDevData *dev = cm::masterDev();
    for(int i=0; i<OUTPUT_NUM; ++i) {
        QString key = QString::number(i+1);
        QString v = "Server" + key;
        QString res = mCfg->readCfg(key, v, prefix).toString();
        qstrcpy(dev->dual.name[i], res.toLatin1().data());
    }
}

void Cfg_ReadParam::groupName()
{
    QString prefix = "GroupName";
    sDevData *dev = cm::masterDev();
    for(int i=0; i<GROUP_NUM; ++i) {
        QString key = QString::number(i+1);
        QString v = "Group" + key;
        QString res = mCfg->readCfg(key, v, prefix).toString();
        qstrcpy(dev->group.name[i], res.toLatin1().data());
    }
}

void Cfg_ReadParam::outputName()
{
    QString prefix = "OutputName";
    sDevData *dev = cm::masterDev();
    for(int i=0; i<OUTPUT_NUM; ++i) {
        QString key = QString::number(i+1);
        QString v = "Output" + key;
        QString res = mCfg->readCfg(key, v, prefix).toString();
        qstrcpy(dev->output.name[i], res.toLatin1().data());
    }
}
