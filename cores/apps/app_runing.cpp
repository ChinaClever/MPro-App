/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_runing.h"
#include "set_core.h"

App_Runing::App_Runing(QObject *parent)
    : App_Smtp{parent}
{
    QTimer::singleShot(755,this,SLOT(runing_initFunSlot()));
}

void App_Runing::runing_initFunSlot()
{
    mTimer = new QTimer(this);
    mTimer->start(1000); cm::masterDev()->pro.core.runSec = 0;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(runing_onTimeoutDone()));
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    sRunTime *run = &(cm::masterDev()->pro.core);
    qstrcpy(run->start, t.toLatin1().data());
}

void App_Runing::runing_onTimeoutDone()
{
    sRunTime *param = &(cm::masterDev()->pro.core);
    param->runSec += 1;
}
