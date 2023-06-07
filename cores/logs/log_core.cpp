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
    //QTimer::singleShot(1367,this, SLOT(invAdcSlot()));
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

Log_Core *Log_Core::bulid(QObject *parent)
{
    static Log_Core *sington = nullptr;
    if(!sington) {
        sington = new Log_Core(parent);
        if(cm::masterDev()->startCnt < 1500) {
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
    QString fn = "/usr/data/clever/cfg/factoryRestore";
    sEventItem it; if(QFile::exists(fn)) {
        if(cm::cn()) {
            it.event_type = QStringLiteral("恢复");
            it.event_content = QStringLiteral("恢复出厂设置");
        } else {
            it.event_type = "restore";
            it.event_content = "restore factory settings";
        } append(it); cm::execute("rm -rf " + fn);
    }
}

void Log_Core::append(const sAlarmItem &it)
{
    QString fmd = "alarm:%1 content:%2";
    QString str = fmd.arg(it.alarm_status, it.alarm_content);
    App_Core::bulid()->smtp_sendMail(str); sys_logAlarm(str);
    Odbc_Core::bulid()->alarm(it);
    mAlarmIts << it; run();
}

void Log_Core::append(const sEventItem &it)
{
    QString fmd = "type:%1 content:%2";
    QString str = fmd.arg(it.event_type, it.event_content);
    sys_logInfo(str); mEventIts << it; run();
    Odbc_Core::bulid()->event(it);
}

void Log_Core::append(const sDataItem &it)
{
    sHdaItem hda;
    hda.addr = QString::number(it.addr);
    hda.type = QString::number(it.type);
    hda.topic = QString::number(it.topic);
    hda.indexes = QString::number(it.id + 1);
    hda.value = QString::number(it.value / cm::decimal(it));
    mHdaIts << hda; run(); //cout << hda.addr << hda.type << hda.topic << hda.indexes << hda.value;
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

void Log_Core::initFunSlot()
{
    sys_initfun();
    factoryRestore();
    mHda = Db_Hda::bulid();
    mOta = Db_Ota::bulid();
    mEvent = Db_Event::bulid();
    mAlarm = Db_Alarm::bulid();
}

void Log_Core::run()
{
    if(!isRun) {
        if(cm::runTime() > 48*60*60) mt += 1567;
        int cnt = mEventIts.size() + mAlarmIts.size();
        if(cnt > 30 || mLogCnt > 1000) {mt += 3567; timeoutDone();} mLogCnt += cnt;
        QTimer::singleShot(mt,this, SLOT(saveLogSlot()));
        //QtConcurrent::run(this, &Log_Core::saveLogSlot);
        isRun = true; mt = 567;
    } else mt = 5567;
}

void Log_Core::saveLogSlot()
{
    QWriteLocker locker(mRwLock);
    QSqlDatabase::database().transaction(); //Db_Tran t;
    while(mOtaIts.size()) mOta->insertItem(mOtaIts.takeFirst());
    while(mHdaIts.size()) mHda->insertItem(mHdaIts.takeFirst());
    while(mEventIts.size()) mEvent->insertItem(mEventIts.takeFirst());
    while(mAlarmIts.size()) mAlarm->insertItem(mAlarmIts.takeFirst());
    QSqlDatabase::database().commit(); cm::mdelay(100);
    system("sync"); isRun = false;
}

void Log_Core::timeoutDone()
{
    QWriteLocker locker(mRwLock);
    int cnt = 1; Db_Tran t; system("sync");
    mHda->countsRemove(cfg.hdaCnt * cnt);
    mAlarm->countsRemove(cfg.logCnt * cnt);
    mEvent->countsRemove(cfg.eventCnt * cnt);
    //QTimer::singleShot(1567,this, SLOT(invAdcSlot()));
    //system("echo 3 > /proc/sys/vm/drop_caches");
}

void Log_Core::invAdcSlot()
{
    QString cmd = "cmd_adc get_voltage 1";
    if(QFile::exists("/usr/bin/cmd_adc")) {
        QString res = cm::execute(cmd); qDebug() << "ADC voltage" << res;
        double vol = res.remove("channel 1:adc sample voltage = ").toDouble();
        if(vol < 11 || vol > 13) {
            sEventItem it; {
                if(cm::cn()) {
                    it.event_type = QStringLiteral("电源检测");
                    it.event_content = QStringLiteral("ADC电压采样异常:%1V").arg(vol);
                } else {
                    it.event_type = "Power Detection";
                    it.event_content = "ADC voltage sampling abnormality ";
                    it.event_content += QString::number(vol) + "V";
                } append(it);
            }
        }
    }
}
