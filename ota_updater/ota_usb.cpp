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


Ota_Usb::Ota_Usb(QObject *parent)
    : Ota_Net{parent}
{
    QTimer::singleShot(1678,this,SLOT(usb_initSlot()));
    system("echo host > /sys/class/usb_role/13500000.otg_new-role-switch/role");
}

void Ota_Usb::usb_initSlot()
{
    QtConcurrent::run(this,&Ota_Usb::usb_run);
    connect(this, &Ota_Usb::usbSig, this, &Ota_Usb::usb_otaSlot);
}

void Ota_Usb::usb_netSLot()
{
    system("echo none > /sys/class/usb_role/13500000.otg_new-role-switch/role");
}

void Ota_Usb::usb_run()
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
        tv.tv_usec = 100 * 1000; tv.tv_sec = 1;
        ret = select(CppLive + 1, &fds, NULL, NULL, &tv);
        if(ret < 0) continue;
        if(!(ret > 0 && FD_ISSET(CppLive, &fds))) continue;
        rcvlen = recv(CppLive, &buf, sizeof(buf), 0); /* receive data */
        if (rcvlen > 0) {            
            if(!isUsbRun) {isUsbRun = true; emit usbSig();} qDebug() << buf;
            /*You can do something here to make the program more perfect!!!*/
        }
    }
    close(CppLive);
}


void Ota_Usb::usb_otaSlot()
{
    QString dir = "/tmp/mass_storage/sda1/clever/";
    cm::mdelay(234); if(QFile::exists(dir + "ver.ini")) {
        sOtaFile it; it.fc = 21; it.path = dir;
        ota_updater(it, DOta_Usb, true);
    } isUsbRun = false;
}
