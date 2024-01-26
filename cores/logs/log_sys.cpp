#include "log_sys.h"
#include "commons.h"
#include <syslog.h>
#include "aiot/aiot_core.h"

#define CLEVER_LOG_TAG "clever"
#define CLEVER_LOG_FLAGS (LOG_CONS | LOG_NDELAY)
#define CLEVER_FACILITY (LOG_LOCAL6)
// #define CLEVER_FACILITY (LOG_USER)

#define CLEVER_OPEN() openlog(CLEVER_LOG_TAG, CLEVER_LOG_FLAGS, CLEVER_FACILITY)
#define CLEVER_EMERG(__fmt, args...) syslog (LOG_EMERG, __fmt, ##args)
#define CLEVER_ALERT(__fmt, args...) syslog (LOG_ALERT, __fmt, ##args)
#define CLEVER_CRIT(__fmt, args...) syslog (LOG_CRIT, __fmt, ##args)
#define CLEVER_ERR(__fmt, args...) syslog (LOG_ERR, __fmt, ##args)
#define CLEVER_WARNING(__fmt, args...) syslog (LOG_WARNING, __fmt, ##args)
#define CLEVER_NOTICE(__fmt, args...) syslog (LOG_NOTICE, __fmt, ##args)
#define CLEVER_INFO(__fmt, args...) syslog (LOG_INFO, __fmt, ##args)
#define CLEVER_DEBUG(__fmt, args...) syslog (LOG_DEBUG, "%s:%d function(%s):" __fmt, __FILE__, __LINE__, __func__,  ##args)
#define CLEVER_CLOSE() closelog()

static QReadWriteLock *gLogLock = new QReadWriteLock;
QReadWriteLock *log_rwLock() {return  gLogLock;}
sSysLogCfg Log_Sys::sysLogCfg;
Log_Sys::Log_Sys(QObject *parent)
    : QThread{parent}
{
    mRwLock = log_rwLock();
}
/**
 * 将日志消息发送到系统日志
 */
void Log_Sys::sys_logInfo(const sEventItem &it)
{
    if(sysLogCfg.en==2){
        //QJsonObject json;
        //json.insert("type", it.event_type);
        //json.insert("content", it.event_content);
        QString uuid = cm::devData(it.addr)->cfg.uut.uuid;

        QJsonObject jsonObject;
        jsonObject.insert("dev", "PDU");
        jsonObject.insert("uuid", uuid);
        //jsonObject.insert("event", json);
        jsonObject.insert("alarm_event", 1);
        jsonObject.insert("state_type", it.event_type);
        jsonObject.insert("content", it.event_content);
        jsonObject.insert("datetime", it.datetime);
        QJsonDocument jsonDocument(jsonObject);
        QString jsonString = jsonDocument.toJson(); // QJsonDocument::Compact
        char *str = jsonString.toUtf8().data(); //cout << msg;
        CLEVER_INFO("%s", str);
    }
}

void Log_Sys::sys_logAlarm(const sAlarmItem &it)
{
    if(sysLogCfg.en) {
        //QJsonObject json;
        //json.insert("status", it.alarm_status);
        //json.insert("content", it.alarm_content);
        QString uuid = cm::devData(it.addr)->cfg.uut.uuid;

        QJsonObject jsonObject;
        jsonObject.insert("dev", "PDU");
        jsonObject.insert("uuid", uuid);
        //jsonObject.insert("alarm", json);
        jsonObject.insert("alarm_event", 2);
        jsonObject.insert("state_type", it.alarm_status);
        jsonObject.insert("content", it.alarm_content);

        jsonObject.insert("datetime", it.datetime);
        QJsonDocument jsonDocument(jsonObject);
        QString jsonString = jsonDocument.toJson(); // QJsonDocument::Compact

        Aiot_Core::bulid()->event_post(jsonString);
        char *str = jsonString.toUtf8().data();
        CLEVER_WARNING("%s", str);
    }
}

void Log_Sys::sys_open()
{
    CLEVER_OPEN();
}

void Log_Sys::sys_initfun()
{
    sSysLogCfg *cfg = &sysLogCfg;
    QString fn = "/usr/data/pdu/bin/";
    if(QFile::exists(fn+"rsyslogcfg") && cfg->en) {
        QString fmd = "rsyslogcfg udp %1 %2 7";
        QString cmd = fmd.arg(cfg->host).arg(cfg->port);
        system(cmd.toLocal8Bit().data()); //cout << cmd;
        QTimer::singleShot(567,this, &Log_Sys::sys_open);
    } else system("killall rsyslogd");
}
