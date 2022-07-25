/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "domain_socketcli.h"

Domain_SocketCli::Domain_SocketCli(QObject *parent)
    : QThread{parent}
{
    initServaddr(m_servaddr);
    m_sockfd = createSocket(); if(m_sockfd > 0) start();
    else qDebug() << "Error: IPC create Socket" << m_sockfd;
}

Domain_SocketCli::~Domain_SocketCli()
{
    close(m_sockfd);
    unlink(UNIXDG_CLI_PATH);
    isRun = false;
    wait();
}

int Domain_SocketCli::send(const QVariant &var)
{
    socklen_t len  = sizeof(m_servaddr);
    QByteArray array = var.toByteArray();
    int ret = sendto(m_sockfd, array.data(), array.size(), 0,
                     (struct sockaddr *)&m_servaddr, len);
    return ret;
}

QVariant Domain_SocketCli::trans(const QVariant &var, int msec)
{
    m_recv.clear();
    int ret = send(var);
    usleep(10); if(ret > 0) {
        for(int i=0; i<msec; ++i) {
            if(m_recv.size()) break; else cm::mdelay(1); //
            //if(i > 2)  qDebug() << "cli _1" << QTime::currentTime().toString("mm:ss zzz") << msec;
        }
    }

    return m_recv;
}

void Domain_SocketCli::initServaddr(sockaddr_un &servaddr)
{
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXDG_SERV_PATH);   /* remote address */
}

int Domain_SocketCli::createSocket()
{
    unlink(UNIXDG_CLI_PATH);
    struct sockaddr_un cliaddr;
    int sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if(sockfd > 0) {
        memset(&cliaddr, 0, sizeof(cliaddr));
        cliaddr.sun_family = AF_LOCAL;
        strcpy(cliaddr.sun_path, UNIXDG_CLI_PATH);
        int ret = bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
        if(ret < 0) if(ret < 0) qDebug() << Q_FUNC_INFO;
    }
    return sockfd;
}

void Domain_SocketCli::workDown()
{
    static char buf[IPC_BUFFER_SIZE] = {0};
    static struct sockaddr_un cliaddr; socklen_t len = sizeof(cliaddr);
    int rtn = recvfrom(m_sockfd, buf, IPC_BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
    if(rtn > 0) m_recv.append(buf, rtn);
}

void Domain_SocketCli::run()
{
    while(isRun) {
        workDown();
        //usleep(1);
    }
}
