/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "log_core.h"
#include "app_core.h"
#include "odbc_core.h"

sLogCfg Log_Core::cfg;
Log_Core::Log_Core(QObject *parent)
    : Log_Read{parent}
{
    uint t = 6*60*60*1000;
    timer = new QTimer(this);
    timer->start(t + rand()%100);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    QTimer::singleShot(65,this,SLOT(initFunSlot()));
}

Log_Core *Log_Core::bulid(QObject *parent)
{
    static Log_Core *sington = nullptr;
    if(!sington) {
        sington = new Log_Core(parent);

        sEventItem it;
        it.event_type = tr("系统事件");
        it.event_content = tr("系统启动");;
        sington->append(it);
    }
    return sington;
}

void Log_Core::append(const sAlarmItem &it)
{
    QString fmd = "alarm:%1 content:%2";
    QString str = fmd.arg(it.alarm_status, it.alarm_content);
    App_Core::bulid()->smtp_sendMail(str); sys_logAlarm(str);
    Odbc_Core::bulid()->alarm(it);
    mAlarmIts << it; start();
}

void Log_Core::append(const sEventItem &it)
{
    QString fmd = "type:%1 content:%2";
    QString str = fmd.arg(it.event_type, it.event_content);
    sys_logInfo(str); mEventIts << it; start();
    Odbc_Core::bulid()->event(it);
}

void Log_Core::append(const sDataItem &it)
{
    sHdaItem hda;
    hda.addr = it.addr;
    hda.type = it.type;
    hda.topic = it.topic;
    hda.index = it.id + 1;
    hda.value = it.value / cm::decimal(it);
    mHdaIts << hda; start();
}

void Log_Core::log_hda(const sDataItem &it)
{
    uint sec = cfg.hdaTime * 60*60;
    if(!cfg.hdaEn || !sec) return ;
    if(!(mCnt%sec)) append(it);
}

void Log_Core::log_hdaEle(const sDataItem &it)
{
    uint sec = cfg.eleTime * 24*60*60;
    if(!cfg.hdaEn || !sec) return ;
    if(!(mCnt%sec)) append(it);
}

void Log_Core::initFunSlot()
{
    sys_initfun();
    mHda = Db_Hda::bulid();
    mOta = Db_Ota::bulid();
    mEvent = Db_Event::bulid();
    mAlarm = Db_Alarm::bulid();
}

void Log_Core::run()
{
    if(!isRun) {
        isRun = true;
        saveLogSlot();
        //start();
        //QTimer::singleShot(350,this, SLOT(saveLogSlot()));
        //QtConcurrent::run(this, &Log_Core::saveLogSlot);
    }
}

void Log_Core::saveLogSlot()
{
    cm::mdelay(350); QWriteLocker locker(mRwLock); Db_Tran t; //cm::mdelay(350);
    while(mOtaIts.size()) mOta->insertItem(mOtaIts.takeFirst());
    while(mHdaIts.size()) mHda->insertItem(mHdaIts.takeFirst());
    while(mEventIts.size()) mEvent->insertItem(mEventIts.takeFirst());
    while(mAlarmIts.size()) mAlarm->insertItem(mAlarmIts.takeFirst());
    isRun = false;
}

void Log_Core::timeoutDone()
{
    int cnt = 10000; Db_Tran t;
    mHda->countsRemove(cfg.hdaCnt * cnt);
    mAlarm->countsRemove(cfg.logCnt * cnt);
    mEvent->countsRemove(cfg.eventCnt * cnt);
    system("echo 3 > /proc/sys/vm/drop_caches");
}
