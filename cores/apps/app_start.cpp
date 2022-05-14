/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_start.h"
#include "op_zrtu.h"
#include "cascade_slave.h"
#include "ipc_coreserver.h"
#include "log_core.h"
#include "data_core.h"
#include "mb_core.h"
#include "ipc_relayclient.h"

App_Start::App_Start(QObject *parent)
    : QObject{parent}
{
    QTimer::singleShot(50,this,SLOT(initFunSlot()));
    QTimer::singleShot(150,this,SLOT(startThreadSlot()));
    //QTimer::singleShot(2500,this,SLOT(clearCacheSlot()));
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
    Set_readWrite::bulid(this);
    Dtls_Recver::bulid(this);
    Alarm_Log::bulid(this);
    Log_Core::bulid(this);    
    Set_Core::bulid();
    OP_Core::bulid();
}

void App_Start::startThreadSlot()
{
    OP_Core::bulid(this)->startFun();

    //QThreadPool *pool = QThreadPool::globalInstance();
    //pool->start(Cascade_Core::bulid(this));
    //pool->start(OP_Core::bulid());
    //pool->start(Mb_Core::bulid(this));
    //pool->start(Data_Core::bulid());
}

void App_Start::clearCacheSlot()
{
    IPC_RelayClient::bulid(this)->ctrl(0, 5, 0);
    //QTimer::singleShot(24*60*60*1000,this,SLOT(clearCacheSlot()));
    //system("sync"); system("echo 3 > /proc/sys/vm/drop_caches");
}
