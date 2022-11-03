#ifndef LOG_CORE_H
#define LOG_CORE_H

#include "log_read.h"

class Log_Core : public Log_Read
{
    Q_OBJECT
    explicit Log_Core(QObject *parent = nullptr);
public:
    static Log_Core *bulid(QObject *parent = nullptr);
    void append(const sEventItem &it) {mEventIts << it; run();}
    void append(const sOtaItem &it) {mOtaIts<<it; run();}
    void append(const sHdaItem &it) {mHdaIts<<it; run();}
    void append(const sAlarmItem &it);

private slots:
    void run();
    void timeoutDone();
    void initFunSlot();
    void saveLogSlot();

private:
    bool isRun=false;
    QTimer *timer;

    Db_Event *mEvent;
    QList<sEventItem> mEventIts;

    Db_Alarm *mAlarm;
    QList<sAlarmItem> mAlarmIts;

    Db_Hda *mHda;
    QList<sHdaItem> mHdaIts;

    Db_Ota *mOta;
    QList<sOtaItem> mOtaIts;
};

#endif // LOG_CORE_H
