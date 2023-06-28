/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_alarmobj.h"
#include "log_core.h"

Cfg_AlarmObj::Cfg_AlarmObj()
{
    isRun = false;
    mData = new cfg::_sDevData;
    mDataStream = new Cfg_AlarmStream(mData);
    memset((void *)mData, 0, sizeof(cfg::_sDevData));
}

void Cfg_AlarmObj::writeAlarms()
{
    if(!isRun) { isRun = true;
        QtConcurrent::run(this,&Cfg_AlarmObj::alarm_run);
    }
}

void Cfg_AlarmObj::saveRelayCnt()
{
    static uint swCnt = 0;
    int size = OUTPUT_NUM; uint cnt = 0;
    uint *data = cm::masterDev()->output.relay.cnt;
    for(int i=0; i<size; ++i) cnt += data[i];
    if(cnt > swCnt) swCnt = cnt; else return;

    QFile file(Cfg_Com::pathOfCfg(CFG_RELAY_FN));
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QDataStream out(&file); out << quint32(size);
        for(int i=0; i<size; ++i) out << data[i];
    }file.close();
}

void Cfg_AlarmObj::readRelayCnt()
{
    int size = OUTPUT_NUM; uint cnt = 0;
    uint *data = cm::masterDev()->output.relay.cnt;
    for(int i=0; i<size; ++i) {cnt += data[i];} if(cnt) return;
    QFile file(Cfg_Com::pathOfCfg(CFG_RELAY_FN));
    if(file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file); in >> size;
        for (int i=0; i<size; i++) in >> data[i];
    } file.close();
}

bool Cfg_AlarmObj::saveAlarms()
{
    cm::mdelay(365); saveRelayCnt();
    QFile file(Cfg_Com::pathOfCfg(CFG_ALARM_FN)); fillData();
    bool ret = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array = toDataStream();
        file.write(qCompress(array));
    }  Alarm_Updater::bulid()->upDevAlarm(0);
    file.close(); system("sync");isRun = false;    
    return ret;
}

bool Cfg_AlarmObj::readAlarm(const QString &fn)
{
    bool ret = false; QFile file(Cfg_Com::pathOfCfg(fn));
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray array = file.readAll();
        array = qUncompress(array);
        if(array.size()) {
            ret = deDataStream(array);
            if(ret) unSequence();
        }
    }

    file.close(); if(!ret) {
        sEventItem it; if(cm::cn()) {
            it.event_type = QStringLiteral("参数异常");
            it.event_content = QStringLiteral("设备报警数据读取异常");
        } else {
            it.event_type = "parameter abnormality ";
            it.event_content = QStringLiteral("Abnormal reading of device alarm data");
        } it.event_content += file.errorString();
        Log_Core::bulid()->append(it);
        cout << Cfg_Com::pathOfCfg(fn);
    } readRelayCnt();

    return ret;
}

QByteArray Cfg_AlarmObj::toDataStream()
{
    QByteArray array; ushort end = END_CRC;
    QDataStream in(&array, QIODevice::WriteOnly);
    in << *mDataStream << end;
    return array;
}

cfg::_sDevData *Cfg_AlarmObj::deDataStream(QByteArray &array)
{
    QDataStream out(&array, QIODevice::ReadOnly);
    ushort end; out >> *mDataStream >> end;
    if(end != END_CRC) {cout << end; return nullptr;}
    return mData;
}
