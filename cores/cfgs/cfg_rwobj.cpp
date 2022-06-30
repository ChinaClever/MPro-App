/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_rwobj.h"

Cfg_RwObj::Cfg_RwObj(QObject *parent) : QObject{parent}
{
    isRun = false;
    mFile = new QFile;
    mData = new cfg::_sDevData;
    mThread = new CThread(this);
    mDataStream = new Cfg_RwStream(mData);
    memset((void *)mData, 0, sizeof(cfg::_sDevData));
}

void Cfg_RwObj::writeSettings()
{
    if(!isRun) {
        isRun = true;
        mThread->onceRun();
    }
}

bool Cfg_RwObj::saveSettings()
{
    mThread->msleep(450);
    mFile->setFileName(Cfg_Obj::pathOfCfg(CFG_DATA_FN)); fillData();
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array = toDataStream();
        mFile->write(array);
    }
    mFile->close();
    isRun = false;
    return ret;
}

bool Cfg_RwObj::readSetting(const QString &fn)
{
    bool ret = false; mFile->setFileName(Cfg_Obj::pathOfCfg(fn));
    if(mFile->exists() && mFile->open(QIODevice::ReadOnly)) {
        QByteArray array = mFile->readAll();
        if(array.size()) {
            ret = deDataStream(array);
            if(ret) unSequence();
        }  mFile->close();
    }

    if(!ret) qCritical() << "Error: read settings" << Cfg_Obj::pathOfCfg(fn)
                         << mFile->errorString() << Q_FUNC_INFO;

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
