/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_devparam.h"

Cfg_devParam::Cfg_devParam()
{
    QString fn = Cfg_Obj::pathOfCfg("devParam.ini");
    mDevCfg = new Cfg_Obj(fn);
    initialParam();
}

void Cfg_devParam::uutInfoRead(sUutInfo &uut)
{
    Cfg_Obj *cfg = mDevCfg; QString g = "uut";
    QString str = cfg->readCfg("room", "", g).toString();
    qstrcpy(uut.room, str.toUtf8().data());

    str = cfg->readCfg("location", "", g).toString();
    qstrcpy(uut.location, str.toUtf8().data());

    str = cfg->readCfg("devName", "", g).toString();
    qstrcpy(uut.devName, str.toUtf8().data());

    str = cfg->readCfg("devType", "MPDU-Pro", g).toString();
    qstrcpy(uut.devType, str.toUtf8().data());

    str = cfg->readCfg("qrcode", "", g).toString();
    qstrcpy(uut.qrcode, str.toUtf8().data());

    str = cfg->readCfg("sn", "", g).toString();
    qstrcpy(uut.sn, str.toUtf8().data());
}

void Cfg_devParam::devNumRead(sDevNums &it)
{
    Cfg_Obj *cfg = mDevCfg; QString g = "devNums";
    it.slaveNum = cfg->readCfg("slaveNum", 0, g).toInt();
    it.boardNum = cfg->readCfg("boardNum", 3, g).toInt();
    it.lineNum = cfg->readCfg("lineNum", LINE_NUM, g).toInt();
    it.loopNum = cfg->readCfg("boardNum", LOOP_NUM/2, g).toInt();
    it.outputNum = cfg->readCfg("outputNum", OUTPUT_NUM/2, g).toInt();

    for(uint i=0; i<it.boardNum; ++i) {
        QString key = "boards_" + QString::number(i);
        it.boards[i] = cfg->readCfg(key, 8, g).toUInt();
    }

    for(uint i=0; i<it.loopNum; ++i) {
        QString key = "loopStarts_" + QString::number(i);
        it.loopStarts[i] = cfg->readCfg(key, 8*i, g).toUInt();

        key = "loopEnds_" + QString::number(i);
        it.loopEnds[i] = cfg->readCfg(key, 8*(i+1), g).toUInt();
    }

    QByteArray array = cfg->readCfg("group", QByteArray(), g).toByteArray();
    if(array.size()) memcpy(it.group, array.data(), sizeof(it.group));
}

void Cfg_devParam::groupWrite()
{
    Cfg_Obj *obj = mDevCfg; QString g = "devNums";
    sDevNums *it = &(cm::masterDev()->cfg.nums);
    QByteArray array = QByteArray::fromRawData((char *)it->group, sizeof(it->group));
    obj->writeCfg("group", array, g);
}

void Cfg_devParam::devParamRead(sParameter &it)
{
    Cfg_Obj *cfg = mDevCfg; QString g = "devParams";
    it.devSpec = cfg->readCfg("devSpec", 4, g).toInt();
    it.language = cfg->readCfg("language", 0, g).toInt();
    it.devMode = cfg->readCfg("devMode", 0, g).toInt();
    it.cascadeAddr = cfg->readCfg("cascadeAddr", 1, g).toInt();
    it.sensorBoxEn = cfg->readCfg("sensorBoxEn", 0, g).toInt();
    it.powLogEn = cfg->readCfg("powLogEn", 0, g).toInt();
    it.buzzerSw = cfg->readCfg("buzzerSw", 1, g).toInt();
    it.drySw = cfg->readCfg("drySw", 0, g).toInt();
    it.isBreaker = cfg->readCfg("isBreaker", 1, g).toInt();
    it.screenAngle = cfg->readCfg("screenAngle", 0, g).toInt();
    it.backlightType = cfg->readCfg("backlightType", 0, g).toInt();
    it.backlightTime = cfg->readCfg("backlightTime", 6, g).toInt();
    it.dataContent = cfg->readCfg("dataContent", 1, g).toInt();
    it.groupEn = cfg->readCfg("groupEn", 0, g).toInt();
    it.runTime = cfg->readCfg("runTime", 0, g).toInt();
    it.vh = cfg->readCfg("vh", 0, g).toInt();
    if(it.buzzerSw) it.buzzerSw = 1;
}

void Cfg_devParam::runTimeWrite()
{
    QString g = "devParams";
    static int cnt=0; bool ret = false;
    sDevCfg *cfg = &(cm::masterDev()->cfg);
    uint t = cfg->param.runTime += 1;
    if(cnt++ < 60) {if(0 == t%10) ret = true;}
    else if(cnt++ < 48*60) {if(0 == t%60) ret = true;}
    else if(0 == t%(24*60)) ret = true;
    if(ret) devParamWrite("runTime", t, g);
}


void Cfg_devParam::devParamWrite(const QString& key, const QVariant &v, const QString& g)
{
     Cfg_Obj *cfg = mDevCfg;
     cfg->writeCfg(key, v, g);
}

void Cfg_devParam::initialParam()
{
    sDevCfg *cfg = &(cm::masterDev()->cfg);
    devNumRead(cfg->nums);
    uutInfoRead(cfg->uut);
    devParamRead(cfg->param);
}

