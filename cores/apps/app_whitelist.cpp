/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_whitelist.h"

sWhiteListCfg App_WhiteList::whiteListCfg;
App_WhiteList::App_WhiteList(QObject *parent)
    : App_Start{parent}
{
    QTimer::singleShot(10,this,&App_WhiteList::internetFirewall);
}

void App_WhiteList::internetFirewall()
{
    bool ret = false; sWhiteListCfg *cfg = &whiteListCfg;
    if(cfg->en && QFile::exists("/usr/data/pdu/bin/netcfg")) {
        system("netcfg -s on"); system("netcfg -a '127.0.0.1 - - -'");
        for(int i=0; i<WHITE_LIST_NUM; ++i) {
            QString ip = cfg->ip[i]; //if(ip.isEmpty()) ip = "-";
            if(ip.size() > 2) {
                QString fmd = "netcfg -a '%1 - - -'";
                QString cmd = fmd.arg(ip);
                system(cmd.toLocal8Bit().data());
                qDebug() << cmd; ret = true;
            }

            QString mac = cfg->mac[i]; //if(mac.isEmpty()) mac = "-";
            if(mac.size() > 2) {
                QString fmd = "netcfg -a '- - - %1'";
                QString cmd = fmd.arg(mac);
                system(cmd.toLocal8Bit().data());
                qDebug() << cmd; ret = true;
            }
        } if(!ret) system("netcfg -s off");
    }
}
