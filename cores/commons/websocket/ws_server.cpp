/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ws_server.h"
#include "sercet_tlscert.h"

WS_Server::WS_Server(QObject *parent) : WS_Object{parent}
{
    m_pWebSocketServer = nullptr;
}

bool WS_Server::initServer(QWebSocketServer::SslMode secureMode, int port)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("WebSocket Server %1").arg(secureMode), secureMode, this);
    connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WS_Server::closed);
    if(QWebSocketServer::SecureMode == secureMode) sslCfg(m_pWebSocketServer);
    return listen(port);
}

void WS_Server::sslCfg(QWebSocketServer *socket)
{
    QSslConfiguration ssl = Sercret_TlsCert::bulid()->sslConfiguration();
    socket->setSslConfiguration(ssl);
}

bool WS_Server::listen(int port)
{
    bool ret = m_pWebSocketServer->listen(QHostAddress::Any, port);
    if(ret) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WS_Server::onNewConnection);
        if(QWebSocketServer::SecureMode ==m_pWebSocketServer->secureMode())
            connect(m_pWebSocketServer, &QWebSocketServer::sslErrors, this, &WS_Server::onSslErrors);
    } else qDebug() << "Error: websocket server listening on port" << port;
    return ret;
}

void WS_Server::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    initSocket(pSocket);
}
