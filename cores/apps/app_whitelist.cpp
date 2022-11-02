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
    QTimer::singleShot(44,this,SLOT(internetFirewall()));
}

void App_WhiteList::internetFirewall()
{
    sWhiteListCfg *cfg = &whiteListCfg;
    if(cfg->en && QFile::exists("netcfg")) {
        system("./netcfg -s on");
        for(int i=0; i<WHITE_LIST_NUM; ++i) {
            if(cfg->ip[i].size())  {
                QString cmd = "./netcfg -a '%1 icmp - -'";
                system(cmd.arg(cfg->ip[i]).toLocal8Bit().data());
            }
            if(cfg->mac[i].size())  {
                QString cmd = "./netcfg -a '- - - %1'";
                system(cmd.arg(cfg->mac[i]).toLocal8Bit().data());
            }
        }
    }
}
