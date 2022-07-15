/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "http.h"

QPair<bool, QByteArray> Http::post(const QString &url, const QByteArray &body, const int &timeout)
{
    const auto &&reply = JQNet::HTTP::post(url, body, timeout);
    //qDebug() << "HTTP post reply:" << reply.first << reply.second;
    return reply;

    //const auto &&reply = JQNet::HTTP::post( "http://127.0.0.1:23412/TestUrl", "BodyData" );
    //qDebug() << "HTTP post reply:" << reply.first << reply.second;

    //const auto &&reply2 = JQNet::HTTP::post( "https://127.0.0.1:23413/TestUrl", "BodyData" );
    //qDebug() << "HTTPS post reply:" << reply2.first << reply2.second;
}
