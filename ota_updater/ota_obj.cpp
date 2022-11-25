/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ota_obj.h"

Ota_Obj::Ota_Obj(QObject *parent)
    : QObject{parent}
{
    system("mkdir -p /tmp/updater/clever");
    QTimer::singleShot(1155,this,SLOT(runing_initFunSlot()));
}

void Ota_Obj::onStartSsdp()
{
    static uint cnt=1, sec=0; if(++cnt%5) return ;
    sRunTime *run = &(cm::masterDev()->proc.core);
    if(run->runSec > sec) {
        sec = run->runSec;
        mSsdp->ssdpClose();
    } else if(!cm::execute("./proc_run cores").toInt()){
        mSsdp->ssdpBind(false);
        //cout << "core stop";
    }
}

void Ota_Obj::runing_initFunSlot()
{
    mTimer = new QTimer(this);
    mTimer->start(1000); mSsdp = new OtaSsdp(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(runing_onTimeoutDone()));
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    sRunTime *run = &(cm::masterDev()->proc.ota);
    qstrcpy(run->start, t.toLatin1().data());
    cm::buildDateTime(run->compileTime);
}

void Ota_Obj::runing_onTimeoutDone()
{
    sRunTime *run = &(cm::masterDev()->proc.ota);
    run->runSec += 1; onStartSsdp(); web_workDown();
}

