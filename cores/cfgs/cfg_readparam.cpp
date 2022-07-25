/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_readparam.h"
#include "agent_core.h"
#include "mb_core.h"
#include "rpc_service.h"
#include "integr_core.h"

Cfg_ReadParam::Cfg_ReadParam(QObject *parent)
    : Cfg_RwInitial{parent}
{
    mCfg = Cfg_Obj::bulid();
}

void Cfg_ReadParam::readCfgParams()
{
    rpc();
    push();
    snmp();
    login();
    modbus();
    readUut();
    outputName();
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

void Cfg_ReadParam::login()
{
    QString prefix = "login";
    QString key; char *ptr=nullptr;
    sDevLogin *it = &(cm::dataPacket()->login);

    for(int i=1; i<4; ++i) {
        switch (i) {
        case 1: key = "user";  ptr = it->user[0]; break;
        case 2: key = "pwd";  ptr = it->pwd[0]; break;
        case 3: key = "token";  ptr = it->token[0]; break;
        case 4: key = "permit";  ptr = it->permit[0]; break;
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
        case 9: key = "sec"; ptr = &cfg->sec; value = 3; break;

        case 11: key = "httpEn"; ptr = &cfg->http.en; value = 0; break;
        case 12: key = "httpUrl"; str = &cfg->http.url; break;
        case 13: key = "httpTimeout"; ptr = &cfg->http.timeout; value = 1;break;
        case 14: key = "enServ"; ptr = &cfg->http.enServ; value = 0; break;
        case 15: key = "httpPort"; ptr = &cfg->http.port; value = 3166;break;
        default: key.clear(); break;
        }

        if(key.size()) {
            if(ptr) *ptr = mCfg->readCfg(key, value, prefix).toInt();
            else *str = mCfg->readCfg(key, value, prefix).toString();
            ptr = nullptr;
        }
    }
}

void Cfg_ReadParam::readUut()
{
    QString prefix = "uut";
    QString key; char *ptr=nullptr;
    sUutInfo *it = &(cm::masterDev()->uut);

    for(int i=1; i<8; ++i) {
        switch (i) {
        case 1: key = "idc";  ptr = it->idc; break;
        case 2: key = "room";  ptr = it->room; break;
        case 3: key = "module";  ptr = it->module; break;
        case 4: key = "cab";  ptr = it->cab; break;
        case 5: key = "road";  ptr = it->road; break;
        case 6: key = "devName";  ptr = it->devName; break;
        case 7: key = "sn";  ptr = it->sn; break;
        }

        QString res = mCfg->readCfg(key, "", prefix).toString();
        qstrcpy(ptr, res.toLatin1().data());
    }


    ///////////=============
    for(int i=0; i<8; ++i) {
        sUutInfo *it = &(cm::devData(i)->uut);
        sprintf(it->devName, "devName_%d", i);
    }
    cm::masterDev()->info.slaveNum = 8;   ///////////=============

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
