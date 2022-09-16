/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_alarmobj.h"
#include "log_core.h"

Cfg_AlarmObj::Cfg_AlarmObj(QObject *parent) : QObject{parent}
{
    isRun = false;
    mFile = new QFile;
    mData = new cfg::_sDevData;
    mThread = new CThread(this);
    mDataStream = new Cfg_AlarmStream(mData);
    memset((void *)mData, 0, sizeof(cfg::_sDevData));
}

void Cfg_AlarmObj::writeAlarms()
{
    if(!isRun) {
        isRun = true;
        mThread->onceRun();
    }
}

bool Cfg_AlarmObj::writeParams()
{
    QFile file(Cfg_Com::pathOfCfg(CFG_PARAM_FN));
    bool ret = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array; ushort end = END_CRC;
        QDataStream in(&array, QIODevice::WriteOnly);
        sDevCfg *cfg = &(cm::masterDev()->cfg);
        in << cm::toByteArray(cfg->nums)
           << cm::toByteArray(cfg->param)
           << cm::toByteArray(cfg->uut) << end;
        file.write(array);
    }
    file.close();
    return ret;
}

bool Cfg_AlarmObj::readParam(const QString &fn)
{
    bool ret = false; QFile file(Cfg_Com::pathOfCfg(fn));
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray array = file.readAll();
        if(array.size()) {
            QByteArray nums, param, uut;
            QDataStream out(&array, QIODevice::ReadOnly);
            ushort end; out >> nums >> param >> uut >> end;
            if(end == END_CRC){
                sDevCfg *cfg = &cm::masterDev()->cfg;
                cfg->nums = cm::toStruct<sDevNums>(nums);
                cfg->param = cm::toStruct<sParameter>(param);
                cfg->uut = cm::toStruct<sUutInfo>(uut); ret = true;
            } else {
                sSysItem it; it.module = tr("配置参数");
                it.content = tr("设备配置参数读取异常:");
                it.content += file.errorString();
                Log_Core::bulid(this)->append(it);
                cout << it.module << it.content << Cfg_Com::pathOfCfg(fn);
            }
        }
    }file.close();

    return ret;
}

bool Cfg_AlarmObj::saveAlarms()
{
    mThread->msleep(450);
    mFile->setFileName(Cfg_Com::pathOfCfg(CFG_ALARM_FN)); fillData();
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array = toDataStream();
        mFile->write(array);
        mFile->flush();
    }
    mFile->close();
    isRun = false;
    return ret;
}

bool Cfg_AlarmObj::readAlarm(const QString &fn)
{
    bool ret = false; mFile->setFileName(Cfg_Com::pathOfCfg(fn));
    if(mFile->exists() && mFile->open(QIODevice::ReadOnly)) {
        QByteArray array = mFile->readAll();
        if(array.size()) {
            ret = deDataStream(array); if(ret) unSequence();
            else {
                sSysItem it; it.module = tr("报警参数");
                it.content = tr("设备报警数据读取异常");
                it.content += mFile->errorString();
                Log_Core::bulid(this)->append(it);
                cout << Cfg_Com::pathOfCfg(fn);
            }
        }  mFile->close();
    }

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
    if(end != END_CRC) return nullptr;
    return mData;
}
