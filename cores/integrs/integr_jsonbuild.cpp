/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_jsonbuild.h"

Integr_JsonBuild::Integr_JsonBuild()
{
    mJson = new QJsonObject();
}

Integr_JsonBuild *Integr_JsonBuild::bulid()
{
    static Integr_JsonBuild* sington = NULL;
    if(sington == NULL) {
        sington = new Integr_JsonBuild();
    }
    return sington;
}

QByteArray Integr_JsonBuild::getJson(uchar addr)
{    
    mDataContent = cm::masterDev()->cfg.param.dataContent;
    QByteArray array; QJsonObject json = getJsonObject(addr);
    if(!json.isEmpty()) {
        QJsonDocument doc(json);
        array = doc.toJson(QJsonDocument::Compact);
    }

    return array;
}

QJsonObject Integr_JsonBuild::getJsonObject(uchar addr)
{
    sDevData *dev = cm::devData(addr);  QJsonObject json;
    //if(!addr) netAddr(cm::dataPacket()->net[0], "net_addr", json);
    if(dev->offLine > 0 || addr == 0) {
        //json.insert("company", "CLEVER");
        faultCode(dev, json);
        json.insert("addr", addr);
        devData(dev, "pdu_data", json);
        json.insert("status", dev->status);
        devInfo(dev->cfg, "pdu_info", json);
        uutInfo(dev->cfg.uut, "uut_info", json);
        verInfo(dev->cfg.vers, "pdu_version", json);
        QDateTime datetime = QDateTime::currentDateTime();
        json.insert("datetime", datetime.toString("yyyy-MM-dd hh:mm:ss"));
        json.insert("version", JSON_VERSION);
    } else {

    }
    return json;
}

void Integr_JsonBuild::saveJson(uchar addr)
{
    QJsonObject json = getJsonObject(addr);
    QString dir = "/tmp/download/dia/metadata/"; cm::execute("mkdir -p "+dir);
    QString fn = dir + QString::number(addr); QFile file(fn+".json");
    bool ret = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QJsonDocument jsonDoc(json);
        QByteArray ba = jsonDoc.toJson(QJsonDocument::Indented);
        file.write(ba); file.close();
    } else {
        cout << "write json file failed";
    }
}

void Integr_JsonBuild::saveJsons()
{
    int size = cm::masterDev()->cfg.nums.slaveNum;
    for(int i=0; i<=size; ++i) saveJson(i);
}

void Integr_JsonBuild::arrayAppend(const uint *ptr, int size,
                                   const QString &key, QJsonObject &json, double r)
{
    QJsonArray array;
    for(int i=0; i<size; ++i) array.append(ptr[i]/r);
    if(array.size()) json.insert(key, QJsonValue(array));
}

void Integr_JsonBuild::faultCode(sDevData *dev, QJsonObject &json)
{
    int size = dev->output.size;
    if(dev->dtc.fault) arrayAppend(dev->dtc.code, size, "fault_code", json);
}

void Integr_JsonBuild::alarmUnit(const sAlarmUnit &it, const QString &key, QJsonObject &json, double r)
{
    int size = it.size; bool dc = false;
    arrayAppend(it.value, size, key+"_value", json, r);
    arrayAppend(it.alarm, size, key+"_alarm_status", json);

    if(mDataContent == 0){
        for(int i=0; i<size; ++i) dc |= it.alarm[i];
    } else if(mDataContent == 1) dc = true;

    if(dc) {
        arrayAppend(it.rated, size, key+"_rated", json, r);
        arrayAppend(it.en, size, key+"_alarm_enable", json);
        arrayAppend(it.min, size, key+"_alarm_min", json, r);
        arrayAppend(it.max, size, key+"_alarm_max", json, r);
        arrayAppend(it.crMin, size, key+"_warn_min", json, r);
        arrayAppend(it.crMax, size, key+"_warn_max", json, r);
    }
}

void Integr_JsonBuild::strListAppend(const char (*ptr)[NAME_SIZE], int size, const QString &key, QJsonObject &json)
{
    QJsonArray array;
    for(int i=0; i<size; ++i) {
        if(strlen(ptr[i])) array.append(ptr[i]);
    } if(array.size()) json.insert(key, array);
}

void Integr_JsonBuild::relayUnit(const sRelayUnit &it, const QString &key, QJsonObject &json)
{
    int size = it.size; bool dc = false;
    arrayAppend(it.sw, size, key+"_state", json);

    if(mDataContent == 0) {
        for(int i=0; i<size; ++i) dc |= it.alarm[i];
    } else if(mDataContent == 1) dc = true;

    if(dc) {
        arrayAppend(it.alarm, size, key+"_alarm", json);
        arrayAppend(it.offAlarm, size, key+"_off_alarm", json);
        arrayAppend(it.resetDelay, size, key+"_reset_delay", json);
        arrayAppend(it.powerUpDelay, size, key+"_powerup_delay", json);
        arrayAppend(it.overrunOff, size, key+"_overrun_off", json);
        arrayAppend(it.timingEn, size, key+"_timing_en", json);
        strListAppend(it.timingOn, size, key+"_timing_on", json);
        strListAppend(it.timingOff, size, key+"_timing_off", json);
    }
}

void Integr_JsonBuild::ObjData(const sObjData &it, const QString &key, QJsonObject &json, int relay)
{
    QJsonObject obj;  int size = it.size;
    alarmUnit(it.vol, "vol", obj, COM_RATE_VOL);
    alarmUnit(it.cur, "cur", obj, COM_RATE_CUR);
    alarmUnit(it.pow, "pow", obj, COM_RATE_POW);

    if(1 == relay) relayUnit(it.relay, "relay", obj);
    else if(2 == relay) arrayAppend(it.relay.sw, it.relay.size, "breaker", obj);

    arrayAppend(it.pf, size, "pf", obj, COM_RATE_PF);
    arrayAppend(it.ele, size, "ele", obj, COM_RATE_ELE);
    arrayAppend(it.artPow, size, "apparent_pow", obj, COM_RATE_POW);
    arrayAppend(it.reactivePow, size, "reactive_pow", obj, COM_RATE_POW);
    if(it.vol.size > 1) arrayAppend(it.lineVol, size, "phase_voltage", obj, COM_RATE_VOL);
    strListAppend(it.name, size, "name", obj);

    json.insert(key, obj);
}

void Integr_JsonBuild::tgUnit(const sTgUnit &it, const QString &key, QJsonObject &json, double r)
{
    QJsonObject obj;
    obj.insert("value", it.value/r);
    obj.insert("rated", it.rated/r);
    obj.insert("alarm_min", it.min/r);
    obj.insert("alarm_max", it.max/r);
    obj.insert("warn_min", it.crMin/r);
    obj.insert("warn_max", it.crMax/r);
    obj.insert("alarm_enable", it.en?true:false);
    json.insert(key, QJsonValue(obj));
}

void Integr_JsonBuild::tgObjData(const sTgObjData &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    if(it.vol.en) tgUnit(it.vol, "vol", obj, COM_RATE_VOL);

    if(it.cur.en) tgUnit(it.cur, "cur", obj, COM_RATE_CUR);
    else obj.insert("cur", it.cur.value/COM_RATE_CUR);

    if(it.pow.en) tgUnit(it.pow, "pow", obj, COM_RATE_POW);
    else  obj.insert("pow", it.pow.value/COM_RATE_POW);

    obj.insert("pf", it.pf/COM_RATE_PF);
    obj.insert("ele", it.ele/COM_RATE_ELE);
    obj.insert("apparent_pow", it.artPow/COM_RATE_POW);
    obj.insert("reactive_pow", it.reactivePow/COM_RATE_POW);
    json.insert(key, QJsonValue(obj));
}

void Integr_JsonBuild::envData(const sEnvData &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    if(it.door[0]||it.door[1])arrayAppend(it.door, 2, "door", obj);
    if(it.water[0]) arrayAppend(it.water, 1, "water", obj);
    if(it.smoke[0]) arrayAppend(it.smoke, 1, "smoke", obj);
    alarmUnit(it.tem, "tem", obj, COM_RATE_TEM);
    alarmUnit(it.hum, "hum", obj, COM_RATE_HUM);
    if(obj.size()) json.insert(key, obj);
}

void Integr_JsonBuild::verInfo(const sVersions &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    if(strlen(it.usr)) obj.insert("usr", it.usr);
    if(strlen(it.md5)) obj.insert("md5", it.md5);
    if(strlen(it.ver)) obj.insert("ver", it.ver);
    if(strlen(it.remark)) obj.insert("remark", it.remark);
    if(strlen(it.oldVersion)) obj.insert("oldVersion", it.oldVersion);
    if(strlen(it.compileDate)) obj.insert("compileDate", it.compileDate);
    if(strlen(it.releaseDate)) obj.insert("releaseDate", it.releaseDate);

    QJsonArray vs;
    for(uint i=0; i<6; ++i) vs.append(it.opVers[i]/10.0);
    obj.insert("op_vers", vs);
    json.insert(key, obj);
}

void Integr_JsonBuild::devInfo(const sDevCfg &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj; double r = 1;
    //obj.insert("pdu_type", "MPDU-Pro");
    obj.insert("line_num", it.nums.lineNum/r);
    obj.insert("pdu_spec", it.param.devSpec/r);

    obj.insert("pdu_hz", it.param.hz/r);
    obj.insert("op_num", it.nums.boardNum/r);
    obj.insert("loop_num", it.nums.loopNum/r);
    obj.insert("slave_num", it.nums.slaveNum/r);
    obj.insert("output_num", it.nums.outputNum/r);

    QJsonArray ops;
    for(uint i=0; i<it.nums.boardNum; ++i) ops.append(it.nums.boards[i]);
    obj.insert("ops_num", ops);
    json.insert(key, obj);
}


void Integr_JsonBuild::uutInfo(const sUutInfo &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    if(strlen(it.sn)) obj.insert("sn", it.sn);
    if(strlen(it.room)) obj.insert("room", it.room);
    if(strlen(it.devName)) obj.insert("name", it.devName);
    if(strlen(it.qrcode)) obj.insert("qrcode", it.qrcode);
    if(strlen(it.devType)) obj.insert("pdu_type", it.devType);
    if(strlen(it.location)) obj.insert("location", it.location);
    if(obj.size()) json.insert(key, obj);
}

void Integr_JsonBuild::devData(sDevData *it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    //it->line.size = it->line.vol.size=it->line.cur.size=it->line.pow.size=3;//////////==========
    ObjData(it->line, "line_item_list", obj);

    //it->loop.size = 6;///////=============
    //it->loop.size = it->loop.vol.size=it->loop.cur.size=it->loop.pow.size=it->loop.relay.size=6;
    ObjData(it->loop, "loop_item_list", obj, 2);

    //it->group.size = 8;/////////////////==============
    //it->group.size = it->group.vol.size=it->group.cur.size=it->group.pow.size=it->group.relay.size=8;
    ObjData(it->group, "group_item_list", obj);

    //it->dual.size = 48;/////////////////============
    //it->dual.size = it->dual.vol.size=it->dual.cur.size=it->dual.pow.size=it->dual.relay.size=48;
    ObjData(it->dual, "dual_item_list", obj);

    //it->output.size = 48;///////////////==========
    //it->output.size = it->output.vol.size=it->output.cur.size=it->output.pow.size=it->output.relay.size=48;
    ObjData(it->output, "output_item_list", obj, 1);

    tgObjData(it->tg, "pdu_tg_data", obj);
    //it->env.tem.size = 2;////////////////===========
    //it->env.hum.size = 2;////////////////============
    envData(it->env, "env_item_list", obj);
    json.insert(key, obj);
}

void Integr_JsonBuild::netAddr(const sNetAddr &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("mode", it.dhcp);
    obj.insert("ip", it.ip);
    obj.insert("mask", it.mask);
    obj.insert("gw", it.gw);
    obj.insert("dns", it.dns);
    //    obj.insert("mac", it.mac);
    json.insert(key, obj);
}
