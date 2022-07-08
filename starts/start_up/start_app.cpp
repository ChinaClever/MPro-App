/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "start_app.h"

Start_App::Start_App(QObject *parent)
    : QObject{parent}
{
    //initUsb();
    QTimer::singleShot(15*1000,this,SLOT(clearCacheSlot()));
}


Start_App *Start_App::bulid(QObject *parent)
{
    static Start_App* sington = nullptr;
    if(sington == nullptr) {
        sington = new Start_App(parent);
    }
    return sington;
}

void Start_App::initUsb()
{
    mUsb = new App_Usb(this);
    mUsb->start();
}


void Start_App::clearCacheSlot()
{
    QTimer::singleShot(24*60*60*1000,this,SLOT(clearCacheSlot()));
    system("sync"); system("echo 3 > /proc/sys/vm/drop_caches");
}
