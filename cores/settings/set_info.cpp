/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_info.h"
//#include "cfg_com.h"
#include "app_core.h"

Set_Info::Set_Info()
{

}

QVariant Set_Info::softwareVersion(int addr, int type)
{
    sDevData *dev = cm::devData(addr);
    sVersions *it = &(dev->cfg.vers);
    QVariant res;

    switch (type) {
    case 0: res = it->dev; break;
    case 1: res = it->ver; break;
    case 2: res = it->md5; break;
    case 3: res = it->usr; break;
    case 4: res = it->remark; break;
    case 5: res = it->oldVersion; break;
    case 6: res = it->compileDate; break;
    case 7: res = it->releaseDate; break;
    case 8: res = it->upgradeDate; break;
    case 11: res = it->opVers[0]; break;
    case 12: res = it->opVers[1]; break;
    case 13: res = it->opVers[2]; break;
    case 14: res = it->opVers[3]; break;
    case 15: res = it->opVers[4]; break;
    case 16: res = it->opVers[5]; break;
    case 21: res = cm::execute("uname -a"); break;
    case 22: res = cm::execute("free"); break;
    case 23: res = cm::execute("df -h /usr/data/"); break;
    case 24: res = cm::execute("ssh -V"); break;
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
    case 5: ret = it->runTime; break;
    case 6: ret = it->hz; break;
    case 7: ret = it->buzzerSw; break;
    case 8: ret = it->groupEn; break;
    case 9: ret = it->sensorBoxEn; break;
    case 10: ret = it->language; break;
    case 11: ret = it->isBreaker; break;
    case 12: ret = it->vh; break;
    case 13: ret = it->screenAngle; break;
    case 14: ret = it->backlightType; break;
    case 15: ret = it->backlightTime; break;
    case 16: App_Core::bulid()->beep_once(); break;
    case 17: ret = it->standNeutral; break;
    case 21: ret = it->jsonContent; break;
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
    case 7: key = "buzzerSw"; it->buzzerSw = value; break;
    case 8: key = "groupEn"; it->groupEn = value; break;
    case 9: key = "sensorBoxEn"; it->sensorBoxEn = value; break;
    case 10: key = "language"; it->language = value; break;
    case 11: key = "isBreaker";  it->isBreaker = value; break;
    case 12: key = "vh"; it->vh = value; break;
    case 13: key = "screenAngle"; it->screenAngle = value; break;
    case 14: key = "backlightType"; it->backlightType = value; break;
    case 15: key = "backlightTime"; it->backlightTime = value; break;
    case 16: App_Core::bulid()->beep_once(); break;
    case 17: key = "standNeutral"; it->standNeutral = value; break;
    case 21: key = "jsonContent"; it->jsonContent = value; break;
    case 22: key = "jsonCompress"; it->jsonCompress = value; break;
    default: ret = false; cout << fc; break;
    } if(ret && key.size()) Cfg_Core::bulid()->devParamWrite(key, value, prefix);
    if(it->cascadeAddr) {sCfgItem it; it.addr = 0; it.type  = 12; it.fc = 5; ret=setCfgNum(it, 0);}
    if((3 == fc) && value) {ret=modbusSet(1, 0);}
    //cout  << key << fc << value;

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
    case 6: value = dev->boards[it.id]; break;
    case 7: value = dev->loopEachNum[it.id]; break;

    //case 7: value = dev->loopEnds[it.id] - dev->loopStarts[it.id]+1; break;
    //case 11: value = dev->loopStarts[it.id]+1;  break;
    //case 12: value = dev->loopEnds[it.id]+1;  break;
    default: cout << it.fc; break;
    } //cout << it.fc << it.id << value;
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
    case 6: key = "boards_" + QString::number(it.id); dev->boards[it.id] = value; break;
    case 7: key = "loopEachNum_" + QString::number(it.id); dev->loopEachNum[it.id] = value; break;
    //case 11: key = "loopStarts_" + QString::number(it.id); dev->loopStarts[it.id] = value;  break;
    //case 12: key = "loopEnds_" + QString::number(it.id); dev->loopEnds[it.id] = value+1;  break;
    default: ret = false; cout << it.fc; break;
    } if(ret && key.size()) Cfg_Core::bulid()->devParamWrite(key, value, prefix);
    cout << key << it.fc << it.id << value;

    if(4 == it.fc) {
        int size = dev->loopNum;
        for(int i=size; i<LOOP_NUM; ++i) dev->loopEachNum[i] = 0;
    } else  if(6 == it.fc) {
        int num = 0; key = "outputNum";
        for(uint i=0; i<dev->boardNum; ++i) num += dev->boards[it.id];
        dev->outputNum = num; Cfg_Core::bulid()->devParamWrite(key, num, prefix);
    }

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
    case 6: ptr = it->devType; break;
    default:  qDebug() << Q_FUNC_INFO; break;
    }

    return ptr;
}

bool Set_Info::setUut(uchar fc, const QVariant &v)
{
    bool ret = true;
    QString prefix = "uut";
    QString key; char *ptr=nullptr;
    QByteArray array = v.toString().toUtf8();
    sUutInfo *it = &(cm::masterDev()->cfg.uut);

    switch (fc) {
    case 1: key = "room";  ptr = it->room; break;
    case 2: key = "location";  ptr = it->location; break;
    case 3: key = "devName";  ptr = it->devName; break;
    case 4: key = "qrcode";  ptr = it->qrcode; qrcodeGenerator(array); break;
    case 5: key = "sn";  ptr = it->sn; break;
    case 6: key = "devType";  ptr = it->devType; break;
    default: ret = false; cout << fc; break;
    }

    if(ptr) {
        qstrcpy(ptr, array.data());
        ptr[array.size()] = 0;
        Cfg_Core *cfg = Cfg_Core::bulid();
        cfg->devParamWrite(key, v, prefix);
    }

    return ret;
}


QString Set_Info::process_log()
{
    QString fn = "/tmp/process_log";
    QFile file(fn); QByteArray array;
    if(file.open(QIODevice::ReadOnly)) {
        array =  file.readAll();
    } file.close();

    return array;
}


QVariant Set_Info::proStartupLog(const sCfgItem &it)
{
    QVariant res;
    sProcState *pro = &(cm::devData(it.addr)->proc);
    sRunTime *run = nullptr; switch (it.fc) {
    case 0: run = &(pro->daemon); break;
    case 1: run = &(pro->core); break;
    case 2: run = &(pro->ota); break;
    case 3: run = &(pro->web); break;
    case 4: run = &(pro->awtk); break;
    case 10: res = process_log(); break;
    default: cout << it.fc; break;
    }

    if(run) {
        switch (it.id) {
        case 1: res = run->runSec; break;
        case 2: res = run->start; break;
        case 3: res = run->md5; break;
        case 4: res = run->compileTime; break;
        case 5: res = run->resetCnt; break;
        default: cout << it.fc; break;
        }
    }

    return res;
}

bool Set_Info::qrcodeGenerator(const QString& msg)
{
    char *ptr = cm::masterDev()->cfg.uut.qrcode;
    qstrcpy(ptr, msg.toUtf8().data());
    if(msg.isEmpty()) ptr[0] = 0;
    cm::qrcodeGenerator(msg);
    return true;
}
