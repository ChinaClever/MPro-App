/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ws_server.h"


WS_Server::WS_Server(QObject *parent) : WS_Object{parent}
{
    m_pWebSocketServer = nullptr;
}

bool WS_Server::initServer(QWebSocketServer::SslMode secureMode, int port)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("WebSocket Server"), secureMode, this);
    connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WS_Server::onNewConnection);
    connect(m_pWebSocketServer, &QWebSocketServer::sslErrors, this, &WS_Server::onSslErrors);
    connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WS_Server::closed);
    if(QWebSocketServer::SecureMode == secureMode) sslCfg();
    return listen(port);
}

void WS_Server::sslCfg()
{
    QSslConfiguration sslConfiguration;
    QFile certFile(QStringLiteral(":/localhost.cert"));
    QFile keyFile(QStringLiteral(":/localhost.key"));
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    m_pWebSocketServer->setSslConfiguration(sslConfiguration);
}

bool WS_Server::listen(int port)
{
    bool ret = m_pWebSocketServer->listen(QHostAddress::Any, port);
    if (!ret) qDebug() << "Error: websocket server listening on port" << port;
    return ret;
}

void WS_Server::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    initSocket(pSocket);
}
