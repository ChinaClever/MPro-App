/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_service.h"
#include "rpc_service.h"
#include "integr_core.h"
#include "sercret_core.h"
#include "mqtt_client.h"
#include "agent_core.h"
#include "app_core.h"
#include "mb_core.h"
#include "qrabbitmq.h"
#include "odbc_obj.h"

Cfg_Service::Cfg_Service()
{
    mCfg = Cfg_Com::bulid();
    readCfgParams();
}

void Cfg_Service::readCfgParams()
{
    log();
    ssh();
    ntp();
    web();
    rpc();
    push();
    snmp();
    smtp();
    mqtt();
    amqp();
    odbc();
    login();
    syslog();
    modbus();
    radius();
    sercret();
    whiteList();
    dualName();
    groupName();
    outputName();
}


void Cfg_Service::smtp()
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
            //case 5: key = "to"; str = &cfg->to; break;
        case 6: key = "port"; ptr = &cfg->port; break;
        case 7: key = "ct"; ptr = &cfg->ct; break;
        default: ptr = nullptr; str = nullptr; break;
        }

        if(str) *str = mCfg->readCfg(key, "", prefix).toString();
        else if(ptr) *ptr = mCfg->readCfg(key, 0, prefix).toInt();
    }

    for(int i=0; i<SMTP_TO_SIZE; ++i) {
        key = "to_" + QString::number(i); str = &cfg->to[i];
        *str = mCfg->readCfg(key, "", prefix).toString();
    }
}

void Cfg_Service::snmp()
{
    sAgentCfg *cfg = &(Agent_Core::snmpCfg);
    QString prefix = "snmp";  QString key;
    QString *str = nullptr; int *ptr = nullptr;
    for(int i=1; i<8; ++i) {
        switch (i) {
        case 1: key = "enV2"; ptr = &cfg->enV2; break;
            //case 2: key = "trap2"; ptr = &cfg->trap2; break;
        case 3: key = "enV3"; ptr = &cfg->enV3; break;
        case 4: key = "usr"; str = &cfg->usr; break;
        case 5: key = "pwd"; str = &cfg->pwd; break;
        case 6: key = "key"; str = &cfg->key; break;
        case 7: key = "encrypt"; str = &cfg->encrypt; break;
        default: ptr = nullptr; break;
        }

        if(str) *str = mCfg->readCfg(key, "", prefix).toString();
        else if(ptr) *ptr = mCfg->readCfg(key, 0, prefix).toInt();
    }

    for(int i=0; i<SNMP_TRAP_SIZE; ++i) {
        key = "trap_" + QString::number(i); str = &cfg->trap[i];
        *str = mCfg->readCfg(key, "", prefix).toString();

        key = "inform_" + QString::number(i); str = &cfg->inform[i];
        *str = mCfg->readCfg(key, "", prefix).toString();
    }
}

void Cfg_Service::web()
{
    ushort *ptr = nullptr; int value = 0;
    QString prefix = "web"; QString key;
    sWebCfg *cfg = &cm::dataPacket()->web;

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

void Cfg_Service::ssh()
{
    sSshCfg *cfg = &App_Ssh::sshCfg;
    QString prefix = "ssh"; QString key;

    for(int i=1; i<5; ++i)  {
        switch (i) {
        case 1: key = "ssh_en";  cfg->ssh_en = mCfg->readCfg(key, 1, prefix).toInt(); break;
        case 2: key = "telnet_en";  cfg->telnet_en = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 3: key = "usr";  cfg->usr = mCfg->readCfg(key, "", prefix).toString();  break;
        case 4: key = "pwd";  cfg->pwd =mCfg->readCfg(key, "", prefix).toString();  break;
        }
    }
}

void Cfg_Service::syslog()
{
    sSysLogCfg *cfg = &Log_Sys::sysLogCfg;
    QString prefix = "syslog"; QString key;

    for(int i=1; i<4; ++i)  {
        switch (i) {
        case 1: key = "en";  cfg->en = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 2: key = "port";  cfg->port = mCfg->readCfg(key, 514, prefix).toInt(); break;
        case 3: key = "host";  cfg->host = mCfg->readCfg(key, "", prefix).toString();  break;
        }
    }
}

void Cfg_Service::radius()
{
    QString prefix = "raduis"; QString key;
    sRadiusCfg *cfg = &App_Radius::radiusCfg;
    for(int i=1; i<7; ++i)  {
        switch (i) {
        case 1: key = "en";  cfg->en = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 2: key = "local";  cfg->local = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 3: key = "host";  cfg->host = mCfg->readCfg(key, "", prefix).toString();  break;
        case 4: key = "key";  cfg->key = mCfg->readCfg(key, "", prefix).toString();  break;
        case 5: key = "authPort";  cfg->authPort = mCfg->readCfg(key, 1813, prefix).toInt(); break;
        case 6: key = "acctPort";  cfg->acctPort = mCfg->readCfg(key, 1812, prefix).toInt(); break;
        }
    }
}

void Cfg_Service::log()
{
    sLogCfg *cfg = &Log_Core::cfg;
    QString prefix = "log"; QString key;

    for(int i=1; i<4; ++i)  {
        switch (i) {
        case 1: key = "eleTime";  cfg->eleTime = mCfg->readCfg(key, 7, prefix).toInt(); break;
        case 2: key = "hdaTime";  cfg->hdaTime = mCfg->readCfg(key, 4, prefix).toInt(); break;
        case 3: key = "logCnt";  cfg->logCnt = mCfg->readCfg(key, 1, prefix).toInt();  break;
        case 4: key = "hdaCnt";  cfg->hdaCnt = mCfg->readCfg(key, 1, prefix).toInt();  break;
        case 5: key = "eventCnt";  cfg->eventCnt = mCfg->readCfg(key, 1, prefix).toInt();  break;
        case 6: key = "hdaEn";  cfg->hdaEn = mCfg->readCfg(key, 0, prefix).toInt();  break;
        }
    }
}

void Cfg_Service::ntp()
{
    sNtpCfg *it = &App_Ntp::ntpCfg;
    QString prefix = "ntp"; QString key;

    for(int i=1; i<5; ++i)  {
        switch (i) {
        case 2: key = "udp_en";  it->udp_en = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 3: key = "ntp_host";  it->ntp_host = mCfg->readCfg(key, "", prefix).toString();  break;
        case 4: key = "time_zone";  it->time_zone =mCfg->readCfg(key, "Asia/Shanghai", prefix).toString();  break;
        }
    }
}

void Cfg_Service::mqtt()
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

void Cfg_Service::amqp()
{
    QString prefix = "amqp"; QString key;
    sAmqpCfg *cfg = &QRabbitMQ::amqpCfg;
    for(int i=1; i<11; ++i) {
        switch (i) {
        case 1: key = "en"; cfg->en = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 2: key = "host"; cfg->host = mCfg->readCfg(key, "", prefix).toString(); break;
        case 3: key = "port"; cfg->port = mCfg->readCfg(key, 15672, prefix).toInt(); break;
        case 4: key = "virtualHost"; cfg->virtualHost = mCfg->readCfg(key, "", prefix).toString(); break;
        case 5: key = "username"; cfg->username = mCfg->readCfg(key, "", prefix).toString(); break;
        case 6: key = "password"; cfg->password = mCfg->readCfg(key, "", prefix).toString(); break;
        case 7: key = "name"; cfg->name = mCfg->readCfg(key, "", prefix).toString(); break;
        case 8: key = "routingKey"; cfg->routingKey = mCfg->readCfg(key, "", prefix).toString();break;
        case 9: key = "bindingKey"; cfg->bindingKey = mCfg->readCfg(key, "", prefix).toString(); break;
        case 10: key = "ssl"; cfg->en = mCfg->readCfg(key, 0, prefix).toInt(); break;
        default: key.clear(); break;
        }
    }
}


void Cfg_Service::odbc()
{
    QString prefix = "odbc"; QString key;
     sOdbcCfg *cfg = &(Odbc_Obj::cfg);
    for(int i=1; i<10; ++i) {
        switch (i) {
        case 1: key = "en"; cfg->en = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 2: key = "host"; cfg->host = mCfg->readCfg(key, "", prefix).toString(); break;
        case 3: key = "port"; cfg->port = mCfg->readCfg(key, 3306, prefix).toInt(); break;
        case 4: key = "db"; cfg->db = mCfg->readCfg(key, "", prefix).toString(); break;
        case 5: key = "user"; cfg->user = mCfg->readCfg(key, "", prefix).toString(); break;
        case 6: key = "pwd"; cfg->pwd = mCfg->readCfg(key, "", prefix).toString(); break;
        case 7: key = "pdukey"; cfg->pdukey = mCfg->readCfg(key, "", prefix).toString(); break;
        case 8: key = "dataPoll"; cfg->dataPoll = mCfg->readCfg(key, "", prefix).toInt();break;
        case 9: key = "hdaPoll"; cfg->hdaPoll = mCfg->readCfg(key, "", prefix).toInt(); break;
        default: key.clear(); break;
        }
    }
}

void Cfg_Service::login()
{
    QString prefix = "login";
    QString key; char *ptr=nullptr;
    sDataPacket *packet = cm::dataPacket();

    for(int k=0; k<USER_NUM; ++k) {
        sDevLogin *it = &(packet->login[k]);
        for(int i=1; i<3; ++i) {
            switch (i) {
            case 1: key = "user_%1";  ptr = it->user; break;
            case 2: key = "pwd_%1";  ptr = it->pwd; break;
            case 3: key = "token_%1";  ptr = it->token; break;
            }
            QString res = mCfg->readCfg(key.arg(k), "", prefix).toString();
            qstrcpy(ptr, res.toLatin1().data());

            key = "permit_%1"; it->permit = mCfg->readCfg(key.arg(k), "", prefix).toInt();
            key = "ctrl_%1"; it->ctrl = mCfg->readCfg(key.arg(k), "", prefix).toLongLong();
        }
    }
}

void Cfg_Service::modbus()
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
    } sParameter *param = &(cm::masterDev()->cfg.param);
    param->modbusRtuAddr = cfg->addr;
    param->modbusRtuBr = cfg->baud;
}

void Cfg_Service::rpc()
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

void Cfg_Service::sercret()
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

void Cfg_Service::whiteList()
{
    QString *str = nullptr;
    QString prefix = "whiteList"; QString key;
    sWhiteListCfg *it = &App_WhiteList::whiteListCfg;
    for(int i=1; i<6; ++i) {
        switch (i) {
        case 1: key = "en"; it->en = mCfg->readCfg(key, 0, prefix).toInt(); break;
        case 2: key = "mac1"; str = &it->mac[0]; break;
        case 3: key = "mac2"; str = &it->mac[1]; break;
        case 4: key = "ip1"; str = &it->ip[0]; break;
        case 5: key = "ip2"; str = &it->ip[1]; break;
        default: key.clear(); break;
        }
        if(str) *str = mCfg->readCfg(key, "", prefix).toString();
    }
}

void Cfg_Service::push()
{
    QString *str = nullptr;
    int *ptr = nullptr; int value = 0;
    QString prefix = "push"; QString key;
    sPushCfg *cfg = &Integr_Core::pushCfg;

    for(int i=1; i<9; ++i) {
        switch (i) {
        case 1: key = "recvEn"; ptr = &cfg->recvEn; value = 0; break;
        case 2: key = "recvPort"; ptr = &cfg->recvPort; value = 3096; break;
        case 3: key = "sec"; ptr = &cfg->sec; value = 5; break;
        case 4: key = "httpEn"; ptr = &cfg->http.en; value = 0; break;
        case 5: key = "httpUrl"; str = &cfg->http.url; break;
        case 6: key = "httpTimeout"; ptr = &cfg->http.timeout; value = 1;break;
        case 7: key = "enServ"; ptr = &cfg->http.enServ; value = 0; break;
        case 8: key = "httpPort"; ptr = &cfg->http.port; value = 3166;break;
        default: key.clear(); break;
        }

        if(key.size()) {
            if(ptr) *ptr = mCfg->readCfg(key, value, prefix).toInt();
            else *str = mCfg->readCfg(key, "", prefix).toString();
            ptr = nullptr;
        }
    }

    for(int i=0; i<INTEGR_UDP_SIZE; ++i) {
        key = "udpEn_" + QString::number(i);
        cfg->udp[i].en = mCfg->readCfg(key, 0, prefix).toInt();

        key = "udpHost_" + QString::number(i);
        cfg->udp[i].host = mCfg->readCfg(key, "", prefix).toString();

        key = "udpPort_" + QString::number(i);
        cfg->udp[i].port = mCfg->readCfg(key, 1124+i, prefix).toInt();
    }
}

void Cfg_Service::dualName()
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

void Cfg_Service::groupName()
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

void Cfg_Service::outputName()
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
