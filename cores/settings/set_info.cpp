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
    sDevCfg *it = &(dev->cfg);
    int ret = 0;

    switch (type) {
    case 0: ret = dev->offLine;  break;
    case 1: ret = it->param.devSpec; break;
    case 2: ret = dev->status; break;
    case 3: ret = it->param.devMode; break;
    case 4: ret = it->param.cascadeAddr; break;
    case 5: ret = it->param.modbusAddr; break;
    case 6: ret = it->param.hz; break;
    case 7: ret = it->param.buzzerSw; break;
    case 8: ret = it->nums.groupEn; break;
    case 9: ret = it->param.runTime; break;
    case 10: ret = it->param.totalTime; break;
    case 11: ret = it->param.isBreaker; break;
    case 12: ret = it->param.vh; break;
    case 13: ret = it->param.screenAngle; break;
    default: cout << type; break;
    }

    return ret;
}


bool Set_Info::setInfoCfg(int addr, int type, int value)
{
    sDevCfg *it = &(cm::devData(addr)->cfg);
    bool ret = true; switch (type) {
    case 1: it->param.devSpec = value; break;
    case 3: it->param.devMode = value; break;
    case 4: it->param.cascadeAddr = value; break;
    case 5: it->param.modbusAddr = value; break;
    case 7: it->param.buzzerSw = value; break;
    case 8: it->nums.groupEn = value; break;
    case 11: it->param.isBreaker = value; break;
    case 12: it->param.vh = value; break;
    case 13: it->param.screenAngle = value; break;
    default: ret = false; cout << type; break;
    } if(ret) Cfg_ReadWrite::bulid()->writeParams();

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
    case 7: value = dev->groupEn; break;
    case 11: value = dev->loopStarts[it.sub];  break;
    case 12: value = dev->loopEnds[it.sub];  break;
    default: cout << it.fc; break;
    }
    return value;
}

bool Set_Info::setCfgNum(const sCfgItem &it, int value)
{    
    sDevNums *dev = &(cm::devData(it.addr)->cfg.nums);
    bool ret = true; switch(it.fc) {
    case DType::Line: dev->lineNum = value; break;
    case DType::Loop: dev->loopNum = value; break;
    case DType::Output: dev->outputNum = value; break;
    case 4: dev->boardNum = value; break;
    case 5: dev->slaveNum = value; break;
    case 6: dev->boards[it.sub] = value; break;
    case 7: dev->groupEn = value; break;
    case 11: dev->loopStarts[it.sub] = value;  break;
    case 12: dev->loopEnds[it.sub] = value;  break;
    default: ret = false; cout << it.fc; break;
    } if(ret) Cfg_ReadWrite::bulid()->writeParams();

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
    //QString prefix = "uut";
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

    if(ptr) qstrcpy(ptr, str);
    if(ret) Cfg_ReadWrite::bulid()->writeParams();
    //Cfg_Obj *cfg = Cfg_Obj::bulid();
    //cfg->writeCfg(key, QString(ptr), prefix);
    // sOpItem db; db.op_src = "uut"; //opSrc(txType);
    // db.content = QStringLiteral("%1 修改为 %2").arg(key, str);
    // Log_Core::bulid()->append(db);

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
