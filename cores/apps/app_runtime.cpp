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
    //QTimer::singleShot(1750,this,&App_RunTime::runing_cpuDone);
}

void App_RunTime::runing_initFunSlot()
{
    mTimer = new QTimer(this); mTimer->start(1000); //mTimer->setTimerType(Qt::PreciseTimer);
    connect(mTimer, &QTimer::timeout, this, &App_RunTime::runing_onTimeoutDone);
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    sRunTime *run = &(cm::masterDev()->proc.core);
    qstrcpy(run->start, t.toLatin1().data());
    cm::buildDateTime(run->compileTime);

    QString fn = "/usr/data/clever/cfg/proc_cnt.conf";
    int cnt = cm::execute("cat "+fn).toInt();
    cm::masterDev()->startCnt = cnt;
}

void App_RunTime::runing_onTimeoutDone()
{
    sRunTime *param = &(cm::masterDev()->proc.core);
    param->runSec += 1; if(0 == (param->runSec % 60)) {
        Cfg_Core::bulid()->runTimeWrite(); system("sync");
    }
}


void App_RunTime::runing_cpuDone()
{
    QString res = cm::execute("cat /tmp/kernel_messages");
    if(res.contains("IRQ Error")) {
            sEventItem it;  if(cm::cn()) {
                it.event_type = QStringLiteral("CPU检测");
                it.event_content = QStringLiteral("内核打印异常");
            } else {
                it.event_type = "CPU Detection";
                it.event_content = "Kernel printing exception ";
            } Log_Core::bulid()->append(it);
    }
}
