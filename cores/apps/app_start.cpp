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

App_Start::App_Start(QObject *parent)
    : App_RunTime{parent}
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
    Rpc_Service::bulid(this);
    Alarm_Log::bulid(this);
    Log_Core::bulid(this);
    Set_Ssdp::bulid(this);
    Set_Core::bulid();
}

void App_Start::startThreadSlot()
{
    Data_Core::bulid();
    Mb_Core::bulid(this);
    Integr_Core::bulid(this);

#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    OP_Core::bulid(this);
    //Agent_Core::bulid(this);
    Cascade_Core::bulid(this);
#endif
}


