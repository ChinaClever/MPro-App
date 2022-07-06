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
#include "integr_core.h"
#include "agent_core.h"
#include "data_core.h"
#include "log_core.h"
#include "set_ssdp.h"
#include "mb_core.h"
#include "op_core.h"

App_Start::App_Start(QObject *parent)
    : QObject{parent}
{
    Shm::initShm();
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
    Integr_Core::bulid(this);
    Alarm_Log::bulid(this);
    Log_Core::bulid(this);
    Set_Ssdp::bulid(this);
    //Cascade_Core::bulid();
    Set_Core::bulid();
    OP_Core::bulid();
}

void App_Start::startThreadSlot()
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    Agent_Core::bulid(this);
#endif

    //OP_Core::bulid(this)->startFun();
    //Cascade_Core::bulid(this)->startFun();

    QThreadPool *pool = QThreadPool::globalInstance();
    pool->start(Mb_Core::bulid(this));
    pool->start(Data_Core::bulid());
}

