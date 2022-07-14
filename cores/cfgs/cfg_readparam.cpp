/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_readparam.h"
#include "mb_core.h"

Cfg_ReadParam::Cfg_ReadParam(QObject *parent)
    : Cfg_RwInitial{parent}
{
    mCfg = Cfg_Obj::bulid();
}

void Cfg_ReadParam::readCfgParams()
{
    login();
    readUut();
    outputName();
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
    sModbusSetting *cfg = Mb_Core::bulid()->cfg();
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
        if(key.size()) *ptr = mCfg->readCfg(key, value, prefix).toInt();
    }
}

void Cfg_ReadParam::readUut()
{
    QString prefix = "uut";
    QString key; char *ptr=nullptr;
    sUutInfo *it = &(cm::masterDev()->uut);

    for(int i=1; i<7; ++i) {
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
