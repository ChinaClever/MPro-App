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
        if(cnt > 10 || mLogCnt > 1000) {mt += 3567; timeoutDone();}
        mLogCnt += cnt; QTimer::singleShot(mt,this, SLOT(saveLogSlot()));
        //QtConcurrent::run(this, &Log_Core::saveLogSlot);
        isRun = true; mt = 567;
    } else mt = 5567;
}

void Log_Core::saveLogSlot()
{
    QWriteLocker locker(mRwLock);
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
    mHda->countsRemove(cfg.hdaCnt * cnt);
    mAlarm->countsRemove(cfg.logCnt * cnt);
    mEvent->countsRemove(cfg.eventCnt * cnt);
    QTimer::singleShot(367,this, &Log_Core::invAdcSlot);
    //system("echo 3 > /proc/sys/vm/drop_caches");
}

void Log_Core::invAdcSlot()
{
    QString cmd = "cmd_adc get_voltage 1";
    if(QFile::exists("/usr/bin/cmd_adc")) {
        QString res = cm::execute(cmd).remove("\n"); qDebug() << "ADC voltage" << res;
        uint vol = res.remove("channel 1:adc sample voltage = ").toUInt() * 18.4;
        cm::masterDev()->cfg.param.supplyVol = vol; //cout << vol;
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
}
