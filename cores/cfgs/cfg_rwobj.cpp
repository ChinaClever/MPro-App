/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_rwobj.h"
#include "log_core.h"

Cfg_RwObj::Cfg_RwObj(QObject *parent) : QObject{parent}
{
    isRun = false;
    mFile = new QFile;
    mData = new cfg::_sDevData;
    mThread = new CThread(this);
    mDataStream = new Cfg_RwStream(mData);
    memset((void *)mData, 0, sizeof(cfg::_sDevData));
}

void Cfg_RwObj::writeAlarms()
{
    if(!isRun) {
        isRun = true;
        mThread->onceRun();
    }
}

bool Cfg_RwObj::writeParams()
{
    QFile file(Cfg_Obj::pathOfCfg(CFG_PARAM_FN));
    bool ret = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array; ushort end = END_CRC;
        QDataStream in(&array, QIODevice::WriteOnly);
        sDevCfg *cfg = &cm::masterDev()->cfg;
        in << cm::toByteArray(cfg->nums)
           << cm::toByteArray(cfg->param)
           << cm::toByteArray(cfg->uut) << end;
        file.write(array);
    }
    mFile->close();
    return ret;
}

bool Cfg_RwObj::readParam(const QString &fn)
{
    bool ret = false; QFile file(Cfg_Obj::pathOfCfg(fn));
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray array = mFile->readAll();
        if(array.size()) {
            QByteArray nums, param, uut;
            QDataStream out(&array, QIODevice::ReadOnly);
            ushort end; out >> nums >> param >> uut >> end;
            if(end == END_CRC){
                sDevCfg *cfg = &cm::masterDev()->cfg;
                cfg->nums = cm::toStruct<sDevNums>(nums);
                cfg->param = cm::toStruct<sParameter>(nums);
                cfg->uut = cm::toStruct<sUutInfo>(nums); ret = true;
            } else {
                sSysItem it; it.module = tr("配置参数");
                it.content = tr("设备配置参数读取异常");
                Log_Core::bulid(this)->append(it);
                qCritical() << "Error: read param" << Cfg_Obj::pathOfCfg(fn)
                             << mFile->errorString() << Q_FUNC_INFO;
            }
        }
    }file.close();

    return ret;
}

bool Cfg_RwObj::saveAlarms()
{
    mThread->msleep(450);
    mFile->setFileName(Cfg_Obj::pathOfCfg(CFG_ALARM_FN)); fillData();
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array = toDataStream();
        mFile->write(array);
    }
    mFile->close();
    isRun = false;
    return ret;
}

bool Cfg_RwObj::readAlarm(const QString &fn)
{
    bool ret = false; mFile->setFileName(Cfg_Obj::pathOfCfg(fn));
    if(mFile->exists() && mFile->open(QIODevice::ReadOnly)) {
        QByteArray array = mFile->readAll();
        if(array.size()) {
            ret = deDataStream(array); if(ret) unSequence();
            else {
                sSysItem it; it.module = tr("配置参数");
                it.content = tr("设备报警数据读取异常");
                Log_Core::bulid(this)->append(it);
                qCritical() << "Error: read alarm" << Cfg_Obj::pathOfCfg(fn)
                                 << mFile->errorString() << Q_FUNC_INFO;
            }
        }  mFile->close();
    }

    return ret;
}

QByteArray Cfg_RwObj::toDataStream()
{
    QByteArray array; ushort end = END_CRC;
    QDataStream in(&array, QIODevice::WriteOnly);
    in << *mDataStream << end;
    return array;
}

cfg::_sDevData *Cfg_RwObj::deDataStream(QByteArray &array)
{
    QDataStream out(&array, QIODevice::ReadOnly);
    ushort end; out >> *mDataStream >> end;
    if(end != END_CRC) return nullptr;
    return mData;
}
