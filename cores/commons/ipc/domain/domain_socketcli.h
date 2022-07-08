#ifndef DOMAIN_SOCKETCLI_H
#define DOMAIN_SOCKETCLI_H

#include "domain_socketserv.h"
#define UNIXDG_CLI_PATH "/tmp/unix_cli.dg"

class Domain_SocketCli : public QThread
{
    Q_OBJECT
public:
    explicit Domain_SocketCli(QObject *parent = nullptr);
    ~Domain_SocketCli();

    int send(const QVariant &var);
    QVariant trans(const QVariant &var, int msec=10);

private:
    void run();
    void workDown();
    int createSocket();
    void initServaddr(sockaddr_un &servaddr);

private:
    int m_sockfd;
    bool isRun=true;
    QByteArray m_recv;
    sockaddr_un m_servaddr;
};

#endif // DOMAIN_SOCKETCLI_H
