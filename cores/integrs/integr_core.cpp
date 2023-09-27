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
    int en = pushCfg.http.enServer; /*HTTPserver开启*/
    int port = pushCfg.http.port;   /*HTTP端口号3066*/
    switch (en) {
    case 1: Integr_HttpServer::initHttpServer(en, port); break; /*启用http服务器*/
    case 2: Integr_HttpServer::initHttpsServer(en, port); break;    /*启用https服务器*/
    } qDebug() << Q_FUNC_INFO << en << port;
}

Integr_Core *Integr_Core::bulid(QObject *parent)
{
    static Integr_Core* sington = NULL;
    if(sington == NULL) {
        sington = new Integr_Core(parent);
    }
    return sington;
}
