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
    uint t = 1*60*60*1000;
    timer = new QTimer(this);
    timer->start(t + rand()%100);
    QTimer::singleShot(65,this,SLOT(initFunSlot()));
    QTimer::singleShot(3367,this, SLOT(timeoutDone()));
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

Log_Core *Log_Core::bulid(QObject *parent)
{
    static Log_Core *sington = nullptr;
    if(!sington) {
        sington = new Log_Core(parent);
        if(cm::masterDev()->startCnt < 1500) {  /*设备启动次数小于1500*/
            sEventItem it; it.addr = 0;
            if(cm::cn()) {
                it.event_type = tr("系统事件");
                it.event_content = tr("系统启动");
            } else {
                it.event_type = "system event";
                it.event_content = "system startup";
            } sington->append(it);
        }
    }
    return sington;
}

void Log_Core::factoryRestore()
{
    QString fn = "/usr/data/clever/cfg/factory_restore.ini";
    QString dst = "/usr/data/clever/cfg/factory_restore.conf";
    sEventItem it; if(QFile::exists(fn)) {
        if(cm::cn()) {
            it.event_type = QStringLiteral("恢复");
            it.event_content = QStringLiteral("恢复出厂设置");
        } else {
            it.event_type = "restore";
            it.event_content = "restore factory settings";
        } append(it); QString fmd = "mv %1 %2";
        cm::execute(fmd.arg(fn, dst));
    }
}

void Log_Core::append(const sAlarmItem &it)
{
    QString fmd = "alarm:%1 content:%2";
    if(cm::cn()) fmd = tr("%1　 内容：%2");
    QString str = fmd.arg(it.alarm_status, it.alarm_content);   /*使用alarm_status和alarm_content填充格式化字符串fmd*/
    App_Core::bulid()->smtp_sendMail(str); sys_logAlarm(it);   /*调用发生SMTP邮件函数和系统告警日志函数*/
    Odbc_Core::bulid()->alarm(it);  /*调用alarm函数，对数据库进行报警记录*/
    mAlarmIts << it; run();     /*将结构体it添加到mAlarmIts中*/
}

void Log_Core::append(const sEventItem &it)
{
    //QString fmd = "type:%1 content:%2";
    //QString str = fmd.arg(it.event_type, it.event_content);     /*使用fmd.arg函数将event_type和event_content的值填充到格式字符串中，生成一个格式化的字符串str*/
    sys_logInfo(it); mEventIts << it; run();   /*run函数控制日志核心的运行操作*/
    Odbc_Core::bulid()->event(it);  /*将事件添加到数据库*/
}

void Log_Core::append(const sDataItem &it)
{
    sHdaItem hda; int offset = 0;
    hda.addr = QString::number(it.addr);
    hda.type = QString::number(it.type);
    hda.topic = QString::number(it.topic);
    hda.indexes = QString::number(it.id + 1);    
    if(DType::Env == it.type && DTopic::Tem == it.topic) if(it.value) offset= 400;
    hda.value = QString::number(((int)it.value-offset) / cm::decimal(it));
    mHdaIts << hda; run();  //cout << hda.addr << hda.type << hda.topic << hda.indexes << hda.value;
}

void Log_Core::log_hda(const sDataItem &it)
{
    uint sec = cfg.hdaTime*60*60;
    //uint sec = cfg.hdaTime*60; //////////////============
    if(!cfg.hdaEn || !sec) return ;
    if(!(mCnt%sec)) append(it);
}

void Log_Core::log_hdaEle(const sDataItem &it)
{
    uint sec = cfg.eleTime*24*60*60;
    //uint sec = cfg.eleTime*60;  ////////////=============
    if(!cfg.hdaEn || !sec) return ;
    if(!(mCnt%sec)) append(it);
}

void Log_Core::timeCheck()
{
    QDateTime t = QDateTime::currentDateTime();
    if(t.date().year() < 2023) {
        sEventItem it; it.addr = 0;
        if(cm::cn()) {
            it.event_type = tr("系统时间错误");
            it.event_content = tr("系统日期晚于2023年");
        } else {
            it.event_type = "System time error";
            it.event_content = "System date is later than 2023";
        } append(it);
    }
}

void Log_Core::initFunSlot()
{
    timeCheck();
    sys_initfun();
    factoryRestore();
    mHda = Db_Hda::bulid();
    mOta = Db_Ota::bulid();
    mEvent = Db_Event::bulid();
    mAlarm = Db_Alarm::bulid();
}
/**
 *控制日志核心的运行操作。如果当前没有正在运行的操作，则根据条件判断执行相应的操作，
 *包括超时处理、计数更新和定时调用槽函数。最后，设置运行状态和mt的值。如果当前有
 *正在运行的操作，则将mt设置为5567，表示等待当前操作完成
 */
void Log_Core::run()
{
    if(!isRun) {
        if(cm::runTime() > 48*60*60) mt += 1567;    /*最近开关运行时间超过48小时*/
        int cnt = mEventIts.size() + mAlarmIts.size();
        if(cnt > 10 || mLogCnt > 1000) {mt += 3567; timeoutDone();}
        mLogCnt += cnt; QTimer::singleShot(mt,this, SLOT(saveLogSlot()));   /*mt秒后调用函数保存日志信息*/
        //QtConcurrent::run(this, &Log_Core::saveLogSlot);
        isRun = true; mt = 567;
    } else mt = 5567;
}
/**
 *提交数据库，将之前的数据库操作永久保存到数据库中
 */
void Log_Core::saveLogSlot()
{
    QWriteLocker locker(mRwLock);   /*加写锁*/
    QSqlDatabase::database().transaction();
    while(mOtaIts.size()) mOta->insertItem(mOtaIts.takeFirst());
    while(mHdaIts.size()) mHda->insertItem(mHdaIts.takeFirst());
    while(mEventIts.size()) mEvent->insertItem(mEventIts.takeFirst());
    while(mAlarmIts.size()) mAlarm->insertItem(mAlarmIts.takeFirst());
    QSqlDatabase::database().commit(); isRun = false; //cm::mdelay(100);
}

void Log_Core::timeoutDone()
{
    int cnt = 1; system("sync");
    QWriteLocker locker(mRwLock); Db_Tran t;
    mHda->countsRemove(cfg.hdaCnt * cnt);   /*超过数据库最大值则删除*/
    mAlarm->countsRemove(cfg.logCnt * cnt);
    mEvent->countsRemove(cfg.eventCnt * cnt);
    QTimer::singleShot(367,this, &Log_Core::invAdcSlot);
    //system("echo 3 > /proc/sys/vm/drop_caches");
}

void Log_Core::invAdcSlot()
{
    uint vol = cm::adcVol();
    if(vol < 11*1000 || vol > 13*1000) {
        sEventItem it;  if(cm::cn()) {
            it.event_type = QStringLiteral("电源检测");
            it.event_content = QStringLiteral("ADC电压采样异常:%1V").arg(vol/1000.0);
        } else {
            it.event_type = "Power Detection";
            it.event_content = "ADC voltage sampling abnormality ";
            it.event_content += QString::number(vol/1000.0) + "V";
        } append(it);
    }
}
