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
    QTimer::singleShot(75,this,&App_RunTime::runing_initFunSlot);
}

void App_RunTime::runing_initFunSlot()
{
    mTimer = new QTimer(this); mTimer->start(1000);
    connect(mTimer, &QTimer::timeout, this, &App_RunTime::runing_onTimeoutDone);
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    sRunTime *run = &(cm::masterDev()->proc.core);
    qstrcpy(run->start, t.toLatin1().data());
    cm::buildDateTime(run->compileTime);

    QString fn = "/usr/data/clever/cfg/proc_cnt.ini";
    int cnt = cm::execute("cat "+fn).toInt();
    cm::masterDev()->startCnt = cnt;
}

void App_RunTime::runing_onTimeoutDone()
{
    sRunTime *param = &(cm::masterDev()->proc.core);
    param->runSec += 1; if(0 == (param->runSec % 60)) {
        Cfg_Core::bulid()->runTimeWrite();        
    }

    if(6 == (param->runSec%(60*60))) { system("sync");
        system("echo 3 > /proc/sys/vm/drop_caches");
    }
}
