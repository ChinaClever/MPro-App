#ifndef LOG_CORE_H
#define LOG_CORE_H

#include "log_read.h"

class Log_Core : public Log_Read
{
    Q_OBJECT
    explicit Log_Core(QObject *parent = nullptr);
public:
    static Log_Core *bulid(QObject *parent = nullptr);
    void append(const sOpItem &it) {mOpIts<<it; run();}
    void append(const sOtaItem &it) {mOtaIts<<it; run();}
    void append(const sEleItem &it) {mEleIts<<it; run();}
    void append(const sSysItem &it) {mSysIts<<it; run();}
    void append(const sUserItem &it) {mUserIts<<it; run();}
    void append(const sAlarmItem &it) {mAlarmIts<<it; run();}
    void append(const sHardwareItem &it) {mHardwareIts<<it; run();}

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

    Db_Ele *mEle;
    QList<sEleItem> mEleIts;

    Db_Ota *mOta;
    QList<sOtaItem> mOtaIts;

    Db_Hardware *mHardware;
    QList<sHardwareItem> mHardwareIts;
};

#endif // LOG_CORE_H
