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
    QTimer::singleShot(1,this,SLOT(internetFirewall()));
}

void App_WhiteList::internetFirewall()
{
    sWhiteListCfg *cfg = &whiteListCfg;
    if(cfg->en && QFile::exists("netcfg")) {
        system("netcfg -s on");
        system("netcfg -a '127.0.0.1 - - -'");
        for(int i=0; i<WHITE_LIST_NUM; ++i) {
            QString ip = cfg->ip[i]; if(ip.isEmpty()) ip = "-";
            QString mac = cfg->mac[i]; if(mac.isEmpty()) mac = "-";
            QString fmd = "netcfg -a '%1 - - %2'";
            QString cmd = fmd.arg(ip, mac);
            system(cmd.toLocal8Bit().data());
            qDebug() << cmd;
        }
    }
}
