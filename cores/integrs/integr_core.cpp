/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_core.h"

Integr_Core::Integr_Core(QObject *parent)
    : Integr_Receiver{parent}
{
    httpServer();
}

void Integr_Core::httpServer()
{
    switch (pushCfg.http.enServ) {
    case 1: Integr_HttpServer::initHttpServer(pushCfg.http.port); break;
    case 2: Integr_HttpServer::initHttpsServer(pushCfg.http.port); break;
    }
    //Integr_HttpServer::initHttpsServer(pushCfg.http.port);   ////=============
}

Integr_Core *Integr_Core::bulid(QObject *parent)
{
    static Integr_Core* sington = NULL;
    if(sington == NULL) {
        sington = new Integr_Core(parent);
        sington->startSlot();
    }
    return sington;
}
