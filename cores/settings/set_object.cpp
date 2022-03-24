#include "set_object.h"

Set_Object::Set_Object(QObject *parent) : QObject{parent}
{
    isRun = false;
    mFile = new QFile;
    mData = new set::_sDevData;
    memset((void *)mData, 0, sizeof(set::_sDevData));
    mDataStream = new Set_Stream(mData);

    mThread = new CThread(this);
    mThread->init(this, SLOT(run()));
}

void Set_Object::writeSettings()
{
    if(!isRun) {isRun = true; QTimer::singleShot(100,mThread, SLOT(onceRun()));}
}

bool Set_Object::saveSettings()
{
    mFile->setFileName("cfg"); fillData();
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
    bool ret = false; mFile->setFileName("cfg");
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
