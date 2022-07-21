/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_info.h"
#include "cfg_obj.h"

Set_Info::Set_Info()
{

}

int Set_Info::devInfoCfg(int addr, int type)
{
    sDevData *dev = cm::devData(addr);
    sDevInfo *it = &(dev->info);
    int ret = 0;

    switch (type) {
    case 0: ret = dev->offLine;  break;
    case 1: ret = it->devSpec; break;
    case 2: ret = it->slaveNum; break;
    case 3: ret = it->modbusAddr; break;
    case 4: ret = it->version; break;
    case 5: ret = it->buzzerSw; break;
    case 6: ret = it->hz; break;
    case 7: ret = it->opNum; break;
    case 8: ret = it->groupEn; break;

    default: qDebug() << Q_FUNC_INFO << type; break;
    }

    return ret;
}


bool Set_Info::setInfoCfg(int addr, int type, int value)
{
    sDevInfo *it = &(cm::devData(addr)->info);
    bool ret = true; switch (type) {
    case 1: it->devSpec = value; break;
    case 2: it->slaveNum = value; break;
    case 3: it->modbusAddr = value; break;
    case 5: it->buzzerSw = value; break;
    case 7: it->opNum = value; break;
    case 8: it->groupEn = value; break;
    default: ret = false; qDebug() << Q_FUNC_INFO << type; break;
    } if(ret) Cfg_ReadWrite::bulid()->writeParams();

    return ret;
}


int Set_Info::devCfgNum(int addr, int type)
{
    sDevInfo *it = &(cm::devData(addr)->info);
    int value = 0; switch(type) {
    case DType::Line: value = it->lineNum; break;
    case DType::Loop: value = it->loopNum; break;
    case DType::Output: value = it->outputNum; break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    }
    return value;
}

bool Set_Info::setCfgNum(int addr, int type, int value)
{
    bool ret = true;
    sDevInfo *it = &(cm::devData(addr)->info);
    uint *ptr = nullptr; switch(type) {
    case DType::Line: ptr = &(it->lineNum); break;
    case DType::Loop: ptr = &(it->loopNum); break;
    case DType::Output: ptr = &(it->outputNum); break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    } if(ret) Cfg_ReadWrite::bulid()->writeParams();

    if(ptr) *ptr = value; else ret = false;
    return ret;
}

QString Set_Info::getUut(int addr, uchar fc)
{
    char *ptr=nullptr;
    sUutInfo *it = &(cm::devData(addr)->uut);

    switch (fc) {
    case 1: ptr = it->idc; break;
    case 2: ptr = it->room; break;
    case 3: ptr = it->module; break;
    case 4: ptr = it->cab; break;
    case 5: ptr = it->road; break;
    case 6: ptr = it->devName; break;
    case 7: ptr = it->sn; break;
    default:  qDebug() << Q_FUNC_INFO; break;
    }

    return ptr;
}

bool Set_Info::setUut(uchar fc, char *str, uchar txType)
{
    bool ret = true;
    QString prefix = "uut";
    QString key; char *ptr=nullptr;
    sUutInfo *it = &(cm::masterDev()->uut);

    switch (fc) {
    case 1: key = "idc";  ptr = it->idc; break;
    case 2: key = "room";  ptr = it->room; break;
    case 3: key = "module";  ptr = it->module; break;
    case 4: key = "cab";  ptr = it->cab; break;
    case 5: key = "road";  ptr = it->road; break;
    case 6: key = "devName";  ptr = it->devName; break;
    case 7: key = "sn";  ptr = it->sn; break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(ptr) qstrcpy(ptr, str);
    Cfg_Obj *cfg = Cfg_Obj::bulid();
    cfg->writeCfg(key, QString(ptr), prefix);

    sOpItem db; db.op_src = opSrc(txType);
    db.content = QObject::tr("%1 修改为 %2").arg(key, str);
    Log_Core::bulid()->append(db);

    return ret;
}
