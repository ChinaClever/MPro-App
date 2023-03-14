/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
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
#include "odbc_core.h"
#include "redis_core.h"

App_Start::App_Start(QObject *parent)
    : App_RunTime{parent}
{
    QTimer::singleShot(5,this, &App_Start::initFunSlot);
    QTimer::singleShot(35,this,&App_Start::startThreadSlot);
    QTimer::singleShot(15145,this,&App_Start::initSysSlot);
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
    Rpc_Service::bulid(this);
    Alarm_Log::bulid(this);
    Set_Ssdp::bulid(this);
    Log_Core::bulid(this);
    Set_Core::bulid();
}

void App_Start::startThreadSlot()
{
    Data_Core::bulid();
    Mb_Core::bulid(this);
    Integr_Core::bulid(this);

#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    Odbc_Core::bulid();
    OP_Core::bulid(this);
    Agent_Core::bulid(this);
    Redis_Core::bulid(this);
    Cascade_Core::bulid(this);
#endif

}


void App_Start::initSysSlot()
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    system("echo 3 > /proc/sys/vm/drop_caches");
#endif
}
