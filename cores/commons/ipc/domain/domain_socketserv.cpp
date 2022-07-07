/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "domain_socketserv.h"

Domain_SocketServ::Domain_SocketServ(QObject *parent)
    : QThread{parent}
{
    m_sockfd = createSocket(); if(m_sockfd > 0) start();
    else qDebug() << "Error: IPC create Socket" << m_sockfd;
}

Domain_SocketServ::~Domain_SocketServ()
{
    close(m_sockfd);
    unlink(UNIXDG_SERV_PATH);
    isRun = false;
    wait();
}

int Domain_SocketServ::createSocket()
{
    unlink(UNIXDG_SERV_PATH);
    struct sockaddr_un servaddr;
    int sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if(sockfd > 0) {
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sun_family = AF_LOCAL;
        strcpy(servaddr.sun_path, UNIXDG_SERV_PATH);
        int ret = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        if(ret < 0) qDebug() << Q_FUNC_INFO;
    }
    return sockfd;
}

void Domain_SocketServ::workDown()
{
    static char buf[IPC_BUFFER_SIZE] = {0};
    static struct sockaddr_un cliaddr; socklen_t len = sizeof(cliaddr);
    int rtn = recvfrom(m_sockfd, buf, IPC_BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
    if(rtn > 0) {
        QVariant res = ipc_reply(QByteArray(buf, rtn));
        QByteArray array = res.toByteArray();
        if(array.size()) {
            rtn = sendto(m_sockfd, array.data(), array.size(), 0,
                         (struct sockaddr *)&cliaddr, len);
            if(rtn<0) qDebug() << Q_FUNC_INFO << rtn << array.size() << cliaddr.sun_path;
        }
        //qDebug() << "serv _1" << QTime::currentTime().toString("mm:ss zzz");
    } else qDebug() << Q_FUNC_INFO << rtn;
}

void Domain_SocketServ::run()
{
    while(isRun) {
        workDown();
        //msleep(1);
    }
}
