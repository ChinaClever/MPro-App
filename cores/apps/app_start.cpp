/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "web_server/web_core.h"
#include "ipc_coreserver.h"
#include "cascade_core.h"
#include "rpc_service.h"
#include "integr_core.h"
#include "agent_core.h"
#include "data_core.h"
#include "app_start.h"
#include "log_core.h"
#include "set_ssdp.h"
#include "mb_core.h"
#include "op_core.h"

App_Start::App_Start(QObject *parent)
    : App_Timing{parent}
{
    QTimer::singleShot(5,this,SLOT(initFunSlot()));
    QTimer::singleShot(15,this,SLOT(startThreadSlot()));
    QThreadPool::globalInstance()->setMaxThreadCount(20);
}

App_Start::~App_Start()
{
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    Shm::delShm();
#endif
}

void App_Start::initFunSlot()
{
    IPC_CoreServer::bulid(this);
    Dtls_Recver::bulid(this);
    Rpc_Service::bulid(this);
    Alarm_Log::bulid(this);
    Log_Core::bulid(this);
    Set_Ssdp::bulid(this);
    Cascade_Core::bulid();
    Integr_Core::bulid();
    Set_Core::bulid();
    OP_Core::bulid();
}

void App_Start::startThreadSlot()
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    Agent_Core::bulid(this);
#endif

    Web_Core::bulid();
    Data_Core::bulid();
    Mb_Core::bulid(this);
    //OP_Core::bulid(this)->startFun();
    Cascade_Core::bulid(this)->startFun();
}


