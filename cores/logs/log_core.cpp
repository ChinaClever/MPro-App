/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "log_core.h"
//#include "commons.h"
#include "app_core.h"

Log_Core::Log_Core(QObject *parent)
    : Log_Read{parent}
{
    uint t = 6*60*60*1000;
    timer = new QTimer(this);
    timer->start(t + rand()%100);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    QTimer::singleShot(65,this,SLOT(initFunSlot()));
}

void Log_Core::append(const sAlarmItem &it)
{
    QString fmd = "addr:%1 state:%2 module:%3 content:%4";
    QString str = fmd.arg(it.addr, it.state, it.module, it.content);
    App_Core::bulid()->smtp_sendMail(str); sys_logAlarm(str);
    mAlarmIts << it;
    run();
}


Log_Core *Log_Core::bulid(QObject *parent)
{
    static Log_Core *sington = nullptr;
    if(!sington) {
        sington = new Log_Core(parent);

        sEventItem it;
        it.type = tr("系统事件");
        it.content = tr("系统启动");;
        sington->append(it);
    }
    return sington;
}

void Log_Core::initFunSlot()
{
    sys_initfun();
    mEvent = Db_Event::bulid();
    mHda = Db_Hda::bulid();
    mOta = Db_Ota::bulid();
    mAlarm = Db_Alarm::bulid();
}

void Log_Core::run()
{
    if(!isRun) {isRun = true; QTimer::singleShot(350,this, SLOT(saveLogSlot()));}
}

void Log_Core::saveLogSlot()
{
    Db_Tran t;
    while(mOtaIts.size()) mOta->insertItem(mOtaIts.takeFirst());
    while(mHdaIts.size()) mHda->insertItem(mHdaIts.takeFirst());
    while(mEventIts.size()) mEvent->insertItem(mEventIts.takeFirst());
    while(mAlarmIts.size()) mAlarm->insertItem(mAlarmIts.takeFirst());
    isRun = false;
}

void Log_Core::timeoutDone()
{
    Db_Tran t;
    int cnt = 10000;
    mHda->countsRemove(cnt);
    mEvent->countsRemove(cnt);
    mAlarm->countsRemove(cnt);
    system("echo 3 > /proc/sys/vm/drop_caches");
}
