/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ota_usb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#define UEVENT_BUFFER_SIZE 2048


static void usb_run()
{
    struct sockaddr_nl client;
    struct timeval tv;
    int CppLive, rcvlen, ret;
    fd_set fds;
    int buffersize = 1024;
    CppLive = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
    memset(&client, 0, sizeof(client));
    client.nl_family = AF_NETLINK;
    client.nl_pid = getpid();
    client.nl_groups = 1; /* receive broadcast message*/
    setsockopt(CppLive, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));
    bind(CppLive, (struct sockaddr*)&client, sizeof(client));
    char buf[UEVENT_BUFFER_SIZE] = { 0 };
    while (1) {
        FD_ZERO(&fds);
        FD_SET(CppLive, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 100 * 1000;
        ret = select(CppLive + 1, &fds, NULL, NULL, &tv);
        if(ret < 0) continue;
        if(!(ret > 0 && FD_ISSET(CppLive, &fds))) continue;
        rcvlen = recv(CppLive, &buf, sizeof(buf), 0); /* receive data */
        if (rcvlen > 0) {
            printf("USB = %s\n", buf);
            /*You can do something here to make the program more perfect!!!*/
        }
    }
    close(CppLive);
}

Ota_Usb::Ota_Usb(QObject *parent)
    : Ota_Net{parent}
{

}
