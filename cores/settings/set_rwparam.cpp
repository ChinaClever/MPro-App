/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_rwparam.h"

Set_RwParam::Set_RwParam(QObject *parent)
    : Set_RwInitial{parent}
{
    mCfg = Cfg_Obj::bulid(CFG_FN);
}

void Set_RwParam::readCfgParams()
{
    readUut();
    outputName();
}

void Set_RwParam::readUut()
{
    QString prefix = "uut";
    QString key; char *ptr=nullptr;
    sUutInfo *it = &(cm::masterDev()->uut);

    for(int i=1; i<7; ++i) {
        switch (i) {
        case 1: key = "idc";  ptr = it->idc; break;
        case 2: key = "room";  ptr = it->room; break;
        case 3: key = "module";  ptr = it->module; break;
        case 4: key = "cab";  ptr = it->cab; break;
        case 5: key = "road";  ptr = it->road; break;
        case 6: key = "devName";  ptr = it->devName; break;
        }

        QString res = mCfg->readCfg(key, "", prefix).toString();
        qstrcpy(ptr, res.toLatin1().data());
    }
}

void Set_RwParam::outputName()
{
    QString prefix = "OutputName";
    sDevData *dev = cm::masterDev();
    for(int i=0; i<OUTPUT_NUM; ++i) {
        QString key = QString::number(i+1);
        QString v = "Output" + key;
        QString res = mCfg->readCfg(key, v, prefix).toString();
        qstrcpy(dev->output.name[i], res.toLatin1().data());
    }
}
