#ifndef ODBC_CORE_H
#define ODBC_CORE_H
#include "odbc_alarm.h"
#include "db_event.h"

class Odbc_Core : public Odbc_Alarm
{    
    explicit Odbc_Core();
public:
    static Odbc_Core *bulid();
    void append(const sOdbcThIt &it);
    void append(const sOdbcHdaIt &it);
    void append(const sOdbcDataIt &it);
    void append(const sOdbcAlarmIt &it);
    void append(const sOdbcEventIt &it);

    void hda(const sDataItem &item);
    void data(const sDataItem &item);
    void alarm(const sAlarmItem &item);
    void event(const sEventItem &item);
    void threshold(const sDataItem &item);

private:
    void run();
    void workDown();
    void insertItems();
    void createTables();

private:
    bool isRun=false;
    QReadWriteLock *mLock;
    QList<sOdbcThIt> mThIts;
    QList<sOdbcHdaIt> mHdaIts;
    QList<sOdbcDataIt> mDataIts;
    QList<sOdbcEventIt> mEventIts;
    QList<sOdbcAlarmIt> mAlarmIts;
};

#endif // ODBC_CORE_H
