/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_rwmain.h"
#include "cfg_obj.h"

Set_RwMain::Set_RwMain(QObject *parent)
    : Set_RwParam{parent}
{
    QTimer::singleShot(15,this,SLOT(readSettings()));
}

Set_RwMain *Set_RwMain::bulid(QObject *parent)
{
    static Set_RwMain* sington = nullptr;
    if(sington == nullptr) {
        sington = new Set_RwMain(parent);
    }
    return sington;
}

void Set_RwMain::writeDefault()
{
    QString cmd = "rm -f %1";
    system(cmd.arg(SET_DEFAULT_FN).toLatin1().data());

    cmd = "cp -rf %1 %2";
    cmd = cmd.arg(SET_DATA_FN, SET_DEFAULT_FN);
    system(cmd.toLatin1().data());
}

bool Set_RwMain::readSettings()
{
    mThread->init(this, SLOT(run()));
    bool ret = readSetting(SET_DATA_FN);
    if(!ret) ret = readDefault();
    if(!ret) ret = initialData();
    readCfgParams();

    return ret;
}

