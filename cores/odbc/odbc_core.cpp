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
    uint sec = cfg.dataPoll;
    if(!cfg.en || !sec) return;
    if(!mCnt % sec) {mDataIts << it; run();}
}

void Odbc_Core::append(const sOdbcEventIt &it)
{
    if(!cfg.en) return;
    mEventIts << it; run();
}

void Odbc_Core::append(const sOdbcHdaIt &it)
{
    uint sec = cfg.hdaPoll * 60;
    if(!cfg.en || !sec) return;
    if(!mCnt % sec) { mHdaIts << it; run();}
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
    if(!cfg.en || !sec) return;

    sOdbcDataIt it;
    it.addr = item.addr;
    it.type = item.type;
    it.topic = item.topic;
    it.indexes = item.id;
    it.value = item.value / cm::decimal(item);
    append(it); hda(item);

    if(1 == cfg.okCnt && item.type != DTopic::Relay) {
        sDataItem dt = item;
        for(int i=DSub::Rated; i<DSub::EnAlarm; ++i) {
            dt.subtopic = i; dt.id += 1;
            Set_Core::bulid()->upMetaData(dt);
            threshold(dt);
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
    if(1 == cfg.okCnt) {
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
    QWriteLocker locker(mLock);
    while(mThIts.size()) th_poll(mThIts.takeFirst());
    while(mHdaIts.size()) hda_insert(mHdaIts.takeFirst());
    while(mDataIts.size()) data_poll(mDataIts.takeFirst());
    while(mEventIts.size()) event_insert(mEventIts.takeFirst());
    while(mAlarmIts.size()) alarm_insert(mAlarmIts.takeFirst());
}

void Odbc_Core::workDown()
{
    cm::mdelay(550);
    bool ret = db_open();
    if(ret) {
        createTables();
        dev_polls();
        insertItems();
    } db_close();
}


void Odbc_Core::run()
{
    if(!isRun) {
        isRun = true;
        QtConcurrent::run(this,&Odbc_Core::workDown);
    }
}
