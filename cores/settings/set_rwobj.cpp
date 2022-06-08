/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_rwobj.h"

Set_RwObj::Set_RwObj(QObject *parent) : QObject{parent}
{
    isRun = false;
    mFile = new QFile;
    mData = new set::_sDevData;
    mThread = new CThread(this);
    mDataStream = new Set_RwStream(mData);
    memset((void *)mData, 0, sizeof(set::_sDevData));
}

void Set_RwObj::writeSettings()
{
    if(!isRun) {
        isRun = true;
        mThread->onceRun();
    }
}

bool Set_RwObj::saveSettings()
{
    mThread->msleep(350);
    mFile->setFileName(Cfg_Obj::pathOfCfg(SET_DATA_FN)); fillData();
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array = toDataStream();
        mFile->write(array);
    }
    mFile->close();
    isRun = false;
    return ret;
}

bool Set_RwObj::readSetting(const QString &fn)
{
    bool ret = false; mFile->setFileName(Cfg_Obj::pathOfCfg(fn));
    if(mFile->exists() && mFile->open(QIODevice::ReadOnly)) {
        QByteArray array = mFile->readAll();
        if(array.size()) {
            if(deDataStream(array)) unSequence();
        }  mFile->close();
    }

    if(!ret) qCritical() << "Error: read settings" << Cfg_Obj::pathOfCfg(fn)
                         << mFile->errorString() << Q_FUNC_INFO;

    return ret;
}

QByteArray Set_RwObj::toDataStream()
{
    QByteArray array; ushort end = END_CRC;
    QDataStream in(&array, QIODevice::WriteOnly);
    in << *mDataStream << end;
    return array;
}

set::_sDevData *Set_RwObj::deDataStream(QByteArray &array)
{
    QDataStream out(&array, QIODevice::ReadOnly);
    ushort end; out >> *mDataStream >> end;
    if(end != END_CRC) return nullptr;
    return mData;
}
