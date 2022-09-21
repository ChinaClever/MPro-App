/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_info.h"
#include "cfg_com.h"

Set_Info::Set_Info()
{

}

QVariant Set_Info::softwareVersion(int addr, int type)
{
    sDevData *dev = cm::devData(addr);
    sVersions *it = &(dev->cfg.vers);
    QVariant res;

    switch (type) {
    case 1: res = it->coreVer; break;
    case 2: res = it->coreCompileTime; break;
    case 3: res = it->coreReleaseTime; break;
    case 4: res = it->startVer; break;
    case 5: res = it->startCompileTime; break;
    case 6: res = it->startReleaseTime; break;
    case 7: res = it->lcdVer; break;
    case 8: res = it->lcdCompileTime; break;
    case 9: res = it->lcdReleaseTime; break;
    case 11: res = it->opVers[0]; break;
    case 12: res = it->opVers[1]; break;
    case 13: res = it->opVers[2]; break;
    case 14: res = it->opVers[3]; break;
    case 15: res = it->opVers[4]; break;
    case 16: res = it->opVers[5]; break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    }
    return res;
}

int Set_Info::devInfoCfg(int addr, int type)
{
    sDevData *dev = cm::devData(addr);
    sParameter *it = &(dev->cfg.param);
    int ret = 0;

    switch (type) {
    case 0: ret = dev->offLine;  break;
    case 1: ret = it->devSpec; break;
    case 2: ret = dev->status; break;
    case 3: ret = it->devMode; break;
    case 4: ret = it->cascadeAddr; break;
    case 5: ret = it->modbusAddr; break;
    case 6: ret = it->hz; break;
    case 7: ret = it->buzzerSw; break;
    case 8: ret = it->groupEn; break;
    case 9: ret = it->runTime; break;
    case 10: ret = it->totalTime; break;
    case 11: ret = it->isBreaker; break;
    case 12: ret = it->vh; break;
    case 13: ret = it->screenAngle; break;
    default: cout << type; break;
    }

    return ret;
}


bool Set_Info::setInfoCfg(int fc, int value)
{
    QString key, prefix = "devParams";
    sParameter *it = &(cm::masterDev()->cfg.param);
    bool ret = true; switch (fc) {
    case 1: key = "devSpec"; it->devSpec = value; break;
    case 3: key = "devMode"; it->devMode = value; break;
    case 4: key = "cascadeAddr"; it->cascadeAddr = value; break;
    case 5: key = "modbusAddr"; it->modbusAddr = value; break;
    case 7: key = "buzzerSw"; it->buzzerSw = value; break;
    case 8: key = "groupEn"; it->groupEn = value; break;
    case 11: key = "isBreaker";  it->isBreaker = value; break;
    case 12: key = "vh"; it->vh = value; break;
    case 13: key = "screenAngle"; it->screenAngle = value; break;
    default: ret = false; cout << fc; break;
    } if(ret) Cfg_ReadWrite::bulid()->devParamWrite(key, value, prefix);

    return ret;
}


int Set_Info::devCfgNum(const sCfgItem &it)
{
    sDevNums *dev = &(cm::devData(it.addr)->cfg.nums);
    int value = 0; switch(it.fc) {
    case DType::Line: value = dev->lineNum; break;
    case DType::Loop: value = dev->loopNum; break;
    case DType::Output: value = dev->outputNum; break;
    case 4: value = dev->boardNum; break;
    case 5: value = dev->slaveNum; break;
    case 6: value = dev->boards[it.sub]; break;
    case 11: value = dev->loopStarts[it.sub];  break;
    case 12: value = dev->loopEnds[it.sub];  break;
    default: cout << it.fc; break;
    }
    return value;
}

bool Set_Info::setCfgNum(const sCfgItem &it, int value)
{    
    QString key, prefix = "devNums";
    sDevNums *dev = &(cm::devData(it.addr)->cfg.nums);
    bool ret = true; switch(it.fc) {
    case DType::Line: key = "lineNum"; dev->lineNum = value; break;
    case DType::Loop: key = "loopNum"; dev->loopNum = value; break;
    case DType::Output: key = "outputNum"; dev->outputNum = value; break;
    case 4: key = "boardNum"; dev->boardNum = value; break;
    case 5: key = "slaveNum"; dev->slaveNum = value; break;
    case 6: key = "boards_" + QString::number(it.sub); dev->boards[it.sub] = value; break;
    case 11: key = "loopStarts_" + QString::number(it.sub); dev->loopStarts[it.sub] = value;  break;
    case 12: key = "loopEnds_" + QString::number(it.sub); dev->loopEnds[it.sub] = value;  break;
    default: ret = false; cout << it.fc; break;
    } if(ret) Cfg_ReadWrite::bulid()->devParamWrite(key, value, prefix);

    return ret;
}

QString Set_Info::getUut(int addr, uchar fc)
{
    char *ptr=nullptr;
    sUutInfo *it = &(cm::devData(addr)->cfg.uut);

    switch (fc) {
    case 1: ptr = it->room; break;
    case 2: ptr = it->location; break;
    case 3: ptr = it->devName; break;
    case 4: ptr = it->qrcode; break;
    case 5: ptr = it->sn; break;
    default:  qDebug() << Q_FUNC_INFO; break;
    }

    return ptr;
}

bool Set_Info::setUut(uchar fc, const QVariant &v)
{
    bool ret = true;
    QString prefix = "uut";
    QString key; char *ptr=nullptr;
    sUutInfo *it = &(cm::masterDev()->cfg.uut);
    char *str = v.toByteArray().data();

    switch (fc) {
    case 1: key = "room";  ptr = it->room; break;
    case 2: key = "location";  ptr = it->location; break;
    case 3: key = "devName";  ptr = it->devName; break;
    case 4: key = "qrcode";  ptr = it->qrcode; qrcodeGenerator(str); break;
    case 5: key = "sn";  ptr = it->sn; break;
    default: ret = false; cout << fc; break;
    }

    if(ptr) {
        qstrcpy(ptr, str);
        Cfg_ReadWrite *cfg = Cfg_ReadWrite::bulid();
        cfg->devParamWrite(key, v, prefix);
    }

    return ret;
}

bool Set_Info::qrcodeGenerator(const QString& msg)
{
    int s = 5; QString fn = "catQR.png";
    char *ptr = cm::masterDev()->cfg.uut.qrcode;
    if(msg.size()) {
        QString cmd = "qrencode -o %1 -s %2 '%3'";
        QString qr = cmd.arg(fn).arg(s).arg(msg);
        qstrcpy(ptr, msg.toLatin1().data());
        system(qr.toLatin1().data());
    } else ptr[0] = 0;

    return true;
}
