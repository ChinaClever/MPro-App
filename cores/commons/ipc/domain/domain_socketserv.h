#ifndef DOMAIN_SOCKETSERV_H
#define DOMAIN_SOCKETSERV_H

#include "shm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIXDG_SERV_PATH "/tmp/unix_serv.dg"
#define UNIXLCD_SERV_PATH "/tmp/unix_serv.lcd"
#define IPC_BUFFER_SIZE 8192

class Domain_SocketServ : public QThread
{
    Q_OBJECT
public:
    explicit Domain_SocketServ(const QString &path, QObject *parent = nullptr);
    ~Domain_SocketServ();

protected:
    virtual QVariant ipc_reply(const QByteArray &array) = 0;

private:
    void run();
    void workDown();
    int createSocket();

private:
    int m_sockfd;
    QString m_path;
    bool isRun=true;
    char *m_buf = nullptr;
};

#endif // DOMAIN_SOCKETSERV_H
