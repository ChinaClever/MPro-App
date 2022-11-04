#include "log_sys.h"
#include "commons.h"
#include <syslog.h>

#define CLEVER_LOG_TAG "MPDU_Pro"
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

sSysLogCfg Log_Sys::sysLogCfg;
Log_Sys::Log_Sys(QObject *parent)
    : QObject{parent}
{

}

void Log_Sys::sys_logInfo(const QString& msg)
{
    char *str = msg.toUtf8().data();
    if(sysLogCfg.en==2) CLEVER_INFO(str);
}

void Log_Sys::sys_logAlarm(const QString& msg)
{
    char *str = msg.toUtf8().data();
    if(sysLogCfg.en) CLEVER_WARNING(str);
}

void Log_Sys::sys_initfun()
{
    sSysLogCfg *cfg = &sysLogCfg;
    if(QFile::exists("rsyslogcfg") && cfg->en) {
        QString fmd = "rsyslogcfg udp %1 %2 7";
        QString cmd = fmd.arg(cfg->host).arg(cfg->port);
        system(cmd.toLocal8Bit().data());
        cm::mdelay(1); CLEVER_OPEN();
    }
}
