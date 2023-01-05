/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_runtime.h"
#include "set_core.h"

App_RunTime::App_RunTime(QObject *parent)
    : App_Ssh{parent}
{
    QTimer::singleShot(75,this,SLOT(runing_initFunSlot()));
}

void App_RunTime::runing_initFunSlot()
{
    mTimer = new QTimer(this); mTimer->start(1000);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(runing_onTimeoutDone()));
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    sRunTime *run = &(cm::masterDev()->proc.core);
    qstrcpy(run->start, t.toLatin1().data());
    cm::buildDateTime(run->compileTime);
}

void App_RunTime::runing_onTimeoutDone()
{
    sRunTime *param = &(cm::masterDev()->proc.core);
    param->runSec += 1; if(0 == (param->runSec % (24*60))) {
        Cfg_Core::bulid()->runTimeWrite();
    }
}
