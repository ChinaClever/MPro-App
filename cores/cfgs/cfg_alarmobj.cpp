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

bool Cfg_AlarmObj::saveAlarms()
{
    cm::mdelay(350); fillData();
    QFile file(Cfg_Com::pathOfCfg(CFG_ALARM_FN));
    bool ret = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array = toDataStream();
        file.write(array);
    } file.close();
    isRun = false;
    return ret;
}

bool Cfg_AlarmObj::readAlarm(const QString &fn)
{
    bool ret = false; QFile file(Cfg_Com::pathOfCfg(fn));
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray array = file.readAll();
        if(array.size()) {
            ret = deDataStream(array); if(ret) unSequence();
            else {
                sEventItem it; it.type = QStringLiteral("参数异常");
                it.content = QStringLiteral("设备报警数据读取异常");
                it.content += file.errorString();
                Log_Core::bulid()->append(it);
                cout << Cfg_Com::pathOfCfg(fn);
            }
        }  file.close();
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
