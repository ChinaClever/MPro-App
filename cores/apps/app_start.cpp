/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_start.h"
#include "cascade_core.h"
#include "ipc_coreserver.h"
#include "rpc_service.h"
#include "ssdp_server.h"
#include "agent_core.h"
#include "data_core.h"
#include "log_core.h"
#include "set_ssdp.h"
#include "mb_core.h"
#include "op_core.h"

App_Start::App_Start(QObject *parent)
    : QObject{parent}
{
    Shm::initShm(); initSystem();
    QTimer::singleShot(50,this,SLOT(initFunSlot()));
    QTimer::singleShot(150,this,SLOT(startThreadSlot()));
    QThreadPool::globalInstance()->setMaxThreadCount(20);
}

App_Start *App_Start::bulid(QObject *parent)
{
    static App_Start* sington = nullptr;
    if(sington == nullptr) {
        sington = new App_Start(parent);
    }
    return sington;
}

void App_Start::initFunSlot()
{
    IPC_CoreServer::bulid(this);
    Dtls_Recver::bulid(this);
    Rpc_Service::bulid(this);
    Alarm_Log::bulid(this);
    Log_Core::bulid(this);
    Set_Ssdp::bulid(this);
    //Cascade_Core::bulid();
    Set_Core::bulid();
    OP_Core::bulid();
}

void App_Start::startThreadSlot()
{
    //Agent_Core::bulid(this);
    //OP_Core::bulid(this)->startFun();
    //Cascade_Core::bulid(this)->startFun();

    QThreadPool *pool = QThreadPool::globalInstance();
    pool->start(Mb_Core::bulid(this));
    pool->start(Data_Core::bulid());
}

void App_Start::initSystem()
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    //initUsb();
    system("ifconfig eth0 up");
    system("route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0");
    QTimer::singleShot(2500,this,SLOT(clearCacheSlot()));
    qDebug() << "AAAAAAAAAAAAA" << App_Run::isRun("http_server");
#endif
}

void App_Start::clearCacheSlot()
{    
    QTimer::singleShot(24*60*60*1000,this,SLOT(clearCacheSlot()));
    system("sync"); system("echo 3 > /proc/sys/vm/drop_caches");
}

void App_Start::initUsb()
{
    mUsb = new App_Usb(this);
    mUsb->start();
}
