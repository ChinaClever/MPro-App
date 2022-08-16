/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "web_server/web_core.h"
#include "ipc_coreserver.h"
#include "cascade_core.h"
#include "sercret_core.h"
#include "rpc_service.h"
#include "ssdp_server.h"
#include "integr_core.h"
#include "agent_core.h"
#include "data_core.h"
#include "app_start.h"
#include "log_core.h"
#include "set_ssdp.h"
#include "mb_core.h"
#include "op_core.h"

App_Start::App_Start(QObject *parent)
    : QObject{parent}
{
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    Shm::initShm();
#endif
    Cfg_ReadWrite::bulid(); compileTime();
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
    //Cascade_Core::bulid(this)->startFun();

}

void App_Start::compileTime()
{
    sVersions *vers = &(cm::masterDev()->cfg.vers);
    QString str = cm::buildDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(vers->coreCompileTime, str.toLatin1().data());
}
