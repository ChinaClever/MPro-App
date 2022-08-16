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
    QByteArray array;
    QJsonObject json = getJsonObject(addr);
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
        json.insert("version", JSON_VERSION);
        devData(dev, "pdu_data", json);
        //saveJson("cc", json);
    } else {

    }
    return json;
}

bool Integr_JsonBuild::saveJson(const QString &name, QJsonObject &json)
{
    QFile file("/home/lzy/" + name+".json");
    bool ret = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QJsonDocument jsonDoc(json);
        QByteArray ba = jsonDoc.toJson(QJsonDocument::Indented);
        file.write(ba); file.close();
    } else {
        qDebug() << "write json file failed";
    }

    return ret;
}


void Integr_JsonBuild::uutInfo(const sUutInfo &it, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("room", it.room);
    obj.insert("cabinet", it.cab);
    obj.insert("road", it.road);
    json.insert("uut_info", QJsonValue(obj));
}

void Integr_JsonBuild::arrayAppend(const uint *ptr, int size,
                                   const QString &key, QJsonObject &json, double r)
{
    QJsonArray array;
    for(int i=0; i<size; ++i) array.append(ptr[i]/r);
    if(array.size()) json.insert(key, QJsonValue(array));
}

void Integr_JsonBuild::alarmUnit(const sAlarmUnit &it, const QString &key, QJsonObject &json, double r)
{
    int size = it.size;
    arrayAppend(it.value, size, key+"_value", json, r);
    arrayAppend(it.en, size, key+"_alarm_enable", json, r);
    //arrayAppend(it.rated, size, key+"_rated", json, r);
    arrayAppend(it.min, size, key+"_alarm_min", json, r);
    arrayAppend(it.max, size, key+"_alarm_max", json, r);
    arrayAppend(it.crMin, size, key+"_warn_min", json, r);
    arrayAppend(it.crMax, size, key+"_warn_max", json, r);
    arrayAppend(it.alarm, size, key+"_alarm_status", json, r);
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
    int size = it.size;
    arrayAppend(it.sw, size, key+"_state", json);
    arrayAppend(it.alarm, size, key+"_alarm", json);
    arrayAppend(it.offAlarm, size, key+"_offAlarm", json);
    arrayAppend(it.resetDelay, size, key+"_resetDelay", json);
    arrayAppend(it.powerUpDelay, size, key+"_powerUpDelay", json);
    arrayAppend(it.overrunOff, size, key+"_overrunOff", json);
    arrayAppend(it.timingEn, size, key+"_timingEn", json);
    strListAppend(it.timingOn, size, key+"_timingOn", json);
    strListAppend(it.timingOff, size, key+"_timingOff", json);
}

void Integr_JsonBuild::ObjData(const sObjData &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    alarmUnit(it.vol, "vol", obj, COM_RATE_VOL);
    alarmUnit(it.cur, "cur", obj, COM_RATE_CUR);
    alarmUnit(it.pow, "pow", obj, COM_RATE_POW);
    relayUnit(it.relay, "relay", obj);

    int size = it.size;
    arrayAppend(it.pf, size, "pf", obj, COM_RATE_PF);
    arrayAppend(it.ele, size, "ele", obj, COM_RATE_ELE);
    arrayAppend(it.artPow, size, "apparent_pow", obj, COM_RATE_PF);
    arrayAppend(it.reactivePow, size, "reactive_pow", obj, COM_RATE_ELE);
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
    tgUnit(it.vol, "vol", obj, COM_RATE_VOL);
    tgUnit(it.cur, "cur", obj, COM_RATE_CUR);
    tgUnit(it.pow, "pow", obj, COM_RATE_POW);

    obj.insert("ele", it.ele/COM_RATE_ELE);
    obj.insert("pf", it.pf/COM_RATE_PF);
    obj.insert("apparent_pow", it.artPow/COM_RATE_POW);
    obj.insert("reactive_pow", it.reactivePow/COM_RATE_POW);
    json.insert(key, QJsonValue(obj));
}

void Integr_JsonBuild::envData(const sEnvData &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    alarmUnit(it.tem, "tem", obj, COM_RATE_TEM);
    alarmUnit(it.hum, "hum", obj, COM_RATE_HUM);
    arrayAppend(it.door, 2, "door", obj);
    arrayAppend(it.water, 1, "water", obj);
    arrayAppend(it.smoke, 1, "smoke", obj);
    json.insert(key, obj);
}

void Integr_JsonBuild::devInfo(const sDevCfg &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj; double r = 1;
    obj.insert("pdu_type", "MPDU-Pro");
    obj.insert("pdu_ver", it.vers.version/r);
    obj.insert("line_num", it.nums.lineNum/r);
    obj.insert("pdu_spec", it.param.devSpec/r);

    obj.insert("pdu_hz", it.param.hz/r);
    obj.insert("op_num", it.nums.boardNum/r);
    obj.insert("loop_num", it.nums.loopNum/r);
    obj.insert("slave_num", it.nums.slaveNum/r);
    obj.insert("output_num", it.nums.outputNum/r);

    obj.insert("core_ver", it.vers.coreVer);
    obj.insert("core_md5", it.vers.coreMd5);
    obj.insert("core_compile_time", it.vers.coreCompileTime);
    obj.insert("lcd_ver", it.vers.lcdVer);
    obj.insert("lcd_md5", it.vers.lcdMd5);
    obj.insert("lcd_compile_time", it.vers.lcdCompileTime);
    obj.insert("start_ver", it.vers.startVer);
    obj.insert("start_md5", it.vers.startMd5);
    obj.insert("start_compile_time", it.vers.startCompileTime);

    QJsonArray ops;
    for(uint i=0; i<it.nums.boardNum; ++i) ops.append(it.nums.boards[i]);
    obj.insert("ops_num", ops);

    QJsonArray vs;
    for(uint i=0; i<it.nums.boardNum; ++i) ops.append(it.vers.opVers[i]);
    obj.insert("op_vers", vs);

    json.insert(key, QJsonValue(obj));
}


void Integr_JsonBuild::uutInfo(const sUutInfo &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("IDC", it.idc);
    obj.insert("room", it.room);
    obj.insert("module", it.module);
    obj.insert("cabinet", it.cab);
    obj.insert("road", it.road);
    obj.insert("name", it.devName);
    obj.insert("sn", it.sn);
    obj.insert("qrcode", it.qrcode);
    json.insert(key, QJsonValue(obj));
}

void Integr_JsonBuild::devData(sDevData *it, const QString &key, QJsonObject &json)/////////////
{
    QJsonObject obj;
    obj.insert("id", it->id);
    obj.insert("alarm", it->alarm?true:false);

    devInfo(it->cfg, "pdu_info", obj);
    uutInfo(it->cfg.uut, "uut_info", obj);

    it->line.size = 3;//////////////////////////////////////////
    ObjData(it->line, "line_item_list", obj);
    it->loop.size = 6;/////////////////////////////////////////
    ObjData(it->loop, "loop_item_list", obj);
    it->group.size = 8;////////////////////////////////////////
    ObjData(it->group, "group_item_list", obj);

    it->output.size = 48;/////////////////////////////////////
    ObjData(it->output, "output_item_list", obj);

    tgObjData(it->tg, "pdu_tg_data", obj);
    envData(it->env, "env_item_list", obj);
    json.insert(key, QJsonValue(obj));
}

void Integr_JsonBuild::netAddr(const sNetAddr &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("mode", it.mode);
    obj.insert("ip", it.ip);
    obj.insert("mask", it.mask);
    obj.insert("gw", it.gw);
    obj.insert("dns", it.dns);
    obj.insert("mac", it.mac);
    json.insert(key, QJsonValue(obj));
}
