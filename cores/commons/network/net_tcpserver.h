#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

#include "net_tcpclient.h"
#include <QTcpServer>

class Net_TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit Net_TcpServer(QObject *parent = nullptr);
    void initServer();

signals:

private:
     QTcpServer *tcpServer = nullptr;
};

#endif // NET_TCPSERVER_H
