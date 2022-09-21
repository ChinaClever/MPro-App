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
    mFile = new QFile;
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

bool Cfg_AlarmObj::saveAlarms()
{
    cm::mdelay(450); fillData();
    mFile->setFileName(Cfg_Com::pathOfCfg(CFG_ALARM_FN));
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
                sSysItem it; it.module = QStringLiteral("报警参数");
                it.content = QStringLiteral("设备报警数据读取异常");
                it.content += mFile->errorString();
                Log_Core::bulid()->append(it);
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
    if(end != END_CRC) {cout << end; return nullptr;}
    return mData;
}
