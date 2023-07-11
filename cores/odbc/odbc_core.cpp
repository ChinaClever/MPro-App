/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_core.h"
#include <QtConcurrent>
#include "set_core.h"
extern QReadWriteLock *log_rwLock();

Odbc_Core::Odbc_Core()
{
    mLock = log_rwLock();
}

Odbc_Core *Odbc_Core::bulid()
{
    static Odbc_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Odbc_Core();
    }
    return sington;
}

void Odbc_Core::append(const sOdbcThIt &it)
{
    if(!cfg.en) return;
    mThIts << it; run();
}

void Odbc_Core::append(const sOdbcAlarmIt &it)
{
    if(!cfg.en) return;
    mAlarmIts << it; run();
}

void Odbc_Core::append(const sOdbcDataIt &it)
{
    uint sec = cfg.dataPoll; if(!cfg.en || !sec) return;
    if(!(mCnt%sec) || 1 == cfg.okCnt){mDataIts << it; run();}
}

void Odbc_Core::append(const sOdbcEventIt &it)
{
    int sec = cm::masterDev()->proc.core.runSec;
    if(!cfg.en || sec < 5) return;
    mEventIts << it; run();
}

void Odbc_Core::append(const sOdbcHdaIt &it)
{
    uint sec = cfg.hdaPoll * 60;
    if(!cfg.en || !sec) return;
    if(!(mCnt%sec)) { mHdaIts << it; run();}
}

void Odbc_Core::hda(const sDataItem &item)
{
    uint sec = cfg.hdaPoll * 60;
    if(!cfg.en || !sec) return;

    sOdbcHdaIt it;
    it.addr = item.addr;
    it.type = item.type;
    it.topic = item.topic;
    it.indexes = item.id;
    it.value = item.value / cm::decimal(item);
    append(it);
}

void Odbc_Core::data(const sDataItem &item)
{
    uint sec = cfg.dataPoll;
    if(!cfg.en || !sec || isDbRun) return;

    sOdbcDataIt it;
    it.addr = item.addr;
    it.type = item.type;
    it.topic = item.topic;
    it.indexes = item.id;
    it.value = item.value / cm::decimal(item);
    append(it); hda(item); //cout << cfg.okCnt << mDataIts.size();

    if((cfg.okCnt<15)  && cfg.status && item.type != DTopic::Relay) {
        sDataItem dt = item;
        for(int i=DSub::Rated; i<DSub::DPeak; ++i) {
            dt.subtopic = i;
            Set_Core::bulid()->upMetaData(dt);
            dt.id = item.id + 1; threshold(dt);
        }
    }
}

void Odbc_Core::threshold(const sDataItem &item)
{
    if(cfg.en) {
        sOdbcThIt it;
        it.addr = item.addr;
        it.type = item.type;
        it.indexes = item.id;
        it.topic = item.topic;
        it.subtopic = item.subtopic;
        it.value = item.value / cm::decimal(item);
        append(it);
    }
}

void Odbc_Core::alarm(const sAlarmItem &item)
{
    if(cfg.en) {
        sOdbcAlarmIt it;
        it.addr = item.addr;
        it.alarm_status = item.alarm_status;
        it.alarm_content = item.alarm_content;
        append(it);
    }
}

void Odbc_Core::event(const sEventItem &item)
{
    if(cfg.en) {
        sOdbcEventIt it;
        it.addr = item.addr;
        it.event_type = item.event_type;
        it.event_content = item.event_content;
        append(it);
    }
}

void Odbc_Core::createTables()
{
    if(cfg.okCnt < 3) {
        index_createTable();
        dev_createTable();
        th_createTable();
        hda_createTable();
        data_createTable();
        alarm_createTable();
        event_createTable();
    }
}

void Odbc_Core::insertItems()
{
    db_transaction(); //QWriteLocker locker(mLock);
    while(mThIts.size()) th_poll(mThIts.takeFirst());
    while(mHdaIts.size()) hda_insert(mHdaIts.takeFirst());
    while(mDataIts.size()) data_poll(mDataIts.takeFirst());
    while(mEventIts.size()) event_insert(mEventIts.takeFirst());
    while(mAlarmIts.size()) alarm_insert(mAlarmIts.takeFirst());
}

void Odbc_Core::clearItems()
{
    mAlarmIts.clear();
    mDataIts.clear();
    mHdaIts.clear();
    mThIts.clear();
}

void Odbc_Core::workDown()
{
    bool ret = db_open();
    if(ret) {
        createTables();
        dev_polls();
        insertItems();
    }else clearItems();
    db_close();
    isRun = false;
}

void Odbc_Core::run()
{
    if(!isRun && cfg.en) {
        isRun = true; cm::mdelay(650);
        QtConcurrent::run(this,&Odbc_Core::workDown);
    }
}
