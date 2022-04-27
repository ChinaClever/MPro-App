#ifndef LOG_CORE_H
#define LOG_CORE_H

#include "db_alarm.h"

class Log_Core : public QObject
{
    Q_OBJECT
    explicit Log_Core(QObject *parent = nullptr);
public:
    static Log_Core *bulid(QObject *parent = nullptr);
    void append(const sOpItem &it) {mOpIts<<it; run();}
    void append(const sSysItem &it) {mSysIts<<it; run();}
    void append(const sUserItem &it) {mUserIts<<it; run();}
    void append(const sAlarmItem &it) {mAlarmIts<<it; run();}

private slots:
    void run();
    void timeoutDone();
    void initFunSlot();
    void saveLogSlot();

private:
    bool isRun=false;
    QTimer *timer;

    Db_Op *mOp;
    QList<sOpItem> mOpIts;

    Db_User *mUser;
    QList<sUserItem> mUserIts;

    Db_Sys *mSys;
    QList<sSysItem> mSysIts;

    Db_Alarm *mAlarm;
    QList<sAlarmItem> mAlarmIts;
};

#endif // LOG_CORE_H
