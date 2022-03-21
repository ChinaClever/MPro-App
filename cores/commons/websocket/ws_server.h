#ifndef WS_SERVER_H
#define WS_SERVER_H

#include "ws_client.h"
#include <QWebSocketServer>
#include <QSslKey>

class WS_Server : public WS_Object
{
    Q_OBJECT
public:
    explicit WS_Server(QObject *parent = nullptr);
    bool initServer(int port) {return initServer(QWebSocketServer::NonSecureMode, port);}
    bool initSslServer(int port) {return initServer(QWebSocketServer::SecureMode, port);}
protected:
    void sslCfg();
    bool listen(int port);
    bool initServer(QWebSocketServer::SslMode secureMode, int port);

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();

private:
    QWebSocketServer *m_pWebSocketServer;
};

#endif // WS_SERVER_H