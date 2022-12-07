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
    bool en = pushCfg.http.en;
    int port = pushCfg.http.port;
    switch (pushCfg.http.enServer) {
    case 1: Integr_HttpServer::initHttpServer(en, port); break;
    case 2: Integr_HttpServer::initHttpsServer(en, port); break;
    }
}

Integr_Core *Integr_Core::bulid(QObject *parent)
{
    static Integr_Core* sington = NULL;
    if(sington == NULL) {
        sington = new Integr_Core(parent);
    }
    return sington;
}
