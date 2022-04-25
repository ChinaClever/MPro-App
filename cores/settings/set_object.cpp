/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_object.h"
#include "cfg_obj.h"
#define SET_FN "set.ini"

Set_Object::Set_Object(QObject *parent) : QObject{parent}
{
    isRun = false;
    mFile = new QFile;
    mData = new set::_sDevData;
    mThread = new CThread(this);
    mDataStream = new Set_Stream(mData);
    memset((void *)mData, 0, sizeof(set::_sDevData));
    QTimer::singleShot(15,this,SLOT(readSettings()));
}

void Set_Object::writeSettings()
{
    if(!isRun) {
        isRun = true;
        mThread->onceRun();
    }
}

bool Set_Object::saveSettings()
{
    mThread->msleep(150);
    mFile->setFileName(Cfg_Obj::pathOfCfg(SET_FN)); fillData();
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array = toDataStream();
        mFile->write(array);
    }
    mFile->close();
    isRun = false;
    return ret;
}

bool Set_Object::readSettings()
{
    mThread->init(this, SLOT(run()));
    bool ret = false; mFile->setFileName(Cfg_Obj::pathOfCfg(SET_FN));
    if(mFile->exists() && mFile->open(QIODevice::ReadOnly)) {
        QByteArray array = mFile->readAll();
        if(array.size()) {
            if(deDataStream(array)) unSequence();
            else qCritical() << "Error: read settings" << __FUNCTION__;
        }  mFile->close();
    }

    return ret;
}

QByteArray Set_Object::toDataStream()
{
    QByteArray array; ushort end = END_CRC;
    QDataStream in(&array, QIODevice::WriteOnly);
    in << *mDataStream << end;
    return array;
}

set::_sDevData *Set_Object::deDataStream(QByteArray &array)
{
    QDataStream out(&array, QIODevice::ReadOnly);
    ushort end; out >> *mDataStream >> end;
    if(end != END_CRC) return nullptr;
    return mData;
}
