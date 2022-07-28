/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "domain_socketserv.h"

Domain_SocketServ::Domain_SocketServ(const QString &path, QObject *parent)
    : QThread{parent}
{
    m_path = path; m_buf = (char *)malloc(IPC_BUFFER_SIZE);
    m_sockfd = createSocket(); if(m_sockfd > 0) start();
    else qDebug() << "Error: IPC create Socket" << m_sockfd;
}

Domain_SocketServ::~Domain_SocketServ()
{
    close(m_sockfd);
    isRun = false; wait();
    unlink(m_path.toLatin1().data());
    free(m_buf);
}

int Domain_SocketServ::createSocket()
{
    struct sockaddr_un servaddr;
    unlink(m_path.toLatin1().data());
    int sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if(sockfd > 0) {
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sun_family = AF_LOCAL;
        strcpy(servaddr.sun_path, m_path.toLatin1().data());
        int ret = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        if(ret < 0) qDebug() << Q_FUNC_INFO;
    }
    return sockfd;
}

void Domain_SocketServ::workDown()
{
    char *buf = m_buf;
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
        //usleep(1);
    }
}
