#ifndef LOG_CORE_H
#define LOG_CORE_H

#include "log_read.h"
struct sLogCfg {
    int hdaEn; // 历史数据是否记录
    int eleTime; // 电能记录间隔时间　天为单位
    int hdaTime; // 历史数据记录时间 单位小时
    int logCnt; // 记录最大条数　默认为1
    int hdaCnt;
    int eventCnt;
};

class Log_Core : public Log_Read
{
    Q_OBJECT
    explicit Log_Core(QObject *parent = nullptr);
public:
    static Log_Core *bulid(QObject *parent = nullptr);
    void append(const sOtaItem &it) {mOtaIts<<it; run();}
    void append(const sEventItem &it);
    void append(const sAlarmItem &it);
    void append(const sDataItem &it);
    void log_hdaEle(const sDataItem &it);
    void log_hda(const sDataItem &it);
    void log_addCnt() {mCnt++;}
    void factoryRestore();
    void timeCheck();
    static sLogCfg cfg;

private slots:
    void run();
    void invAdcSlot();
    void timeoutDone();
    void initFunSlot();
    void saveLogSlot();

private:
    bool isRun=false;
    QTimer *timer;
    uint mCnt = 1;
    uint mt = 567;
    uint mLogCnt =1;

    Db_Hda *mHda;
    QList<sHdaItem> mHdaIts;

    Db_Event *mEvent;
    QList<sEventItem> mEventIts;

    Db_Alarm *mAlarm;
    QList<sAlarmItem> mAlarmIts;

    Db_Ota *mOta;
    QList<sOtaItem> mOtaIts;
};

#endif // LOG_CORE_H
