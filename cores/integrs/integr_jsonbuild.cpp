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
        faultCode(dev, json);
        json.insert("addr", addr);
        devData(dev, "pdu_data", json);
        devInfo(dev->cfg, "pdu_info", json);
        uutInfo(dev->cfg.uut, "uut_info", json);
        verInfo(dev->cfg.vers, "pdu_version", json);
        json.insert("alarm", dev->alarm?true:false);
        QDateTime datetime = QDateTime::currentDateTime();
        json.insert("datetime", datetime.toString("yyyy-MM-dd hh:mm:ss"));
        json.insert("version", JSON_VERSION);
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
    int size = it.size;
    arrayAppend(it.value, size, key+"_value", json, r);
    arrayAppend(it.en, size, key+"_alarm_enable", json, r);
    arrayAppend(it.rated, size, key+"_rated", json, r);
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
    arrayAppend(it.offAlarm, size, key+"_off_alarm", json);
    arrayAppend(it.resetDelay, size, key+"_reset_delay", json);
    arrayAppend(it.powerUpDelay, size, key+"_powerup_delay", json);
    arrayAppend(it.overrunOff, size, key+"_overrun_off", json);
    arrayAppend(it.timingEn, size, key+"_timing_en", json);
    strListAppend(it.timingOn, size, key+"_timing_on", json);
    strListAppend(it.timingOff, size, key+"_timing_off", json);
}

void Integr_JsonBuild::ObjData(const sObjData &it, const QString &key, QJsonObject &json, bool breaker)
{
    QJsonObject obj;  int size = it.size;
    alarmUnit(it.vol, "vol", obj, COM_RATE_VOL);
    alarmUnit(it.cur, "cur", obj, COM_RATE_CUR);
    alarmUnit(it.pow, "pow", obj, COM_RATE_POW);
    if(0 == breaker) relayUnit(it.relay, "relay", obj);
    else arrayAppend(it.relay.sw, size, "breaker", obj);

    arrayAppend(it.pf, size, "pf", obj, COM_RATE_PF);
    arrayAppend(it.ele, size, "ele", obj, COM_RATE_ELE);
    arrayAppend(it.artPow, size, "apparent_pow", obj, COM_RATE_POW);
    arrayAppend(it.reactivePow, size, "reactive_pow", obj, COM_RATE_POW);
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
    //    tgUnit(it.vol, "vol", obj, COM_RATE_VOL);
    //    tgUnit(it.cur, "cur", obj, COM_RATE_CUR);
    //    tgUnit(it.pow, "pow", obj, COM_RATE_POW);

    obj.insert("pf", it.pf/COM_RATE_PF);
    obj.insert("ele", it.ele/COM_RATE_ELE);
    obj.insert("cur", it.cur.value/COM_RATE_CUR);
    obj.insert("pow", it.pow.value/COM_RATE_POW);
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

void Integr_JsonBuild::verInfo(const sVersions &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    if(strlen(it.coreVer)) obj.insert("core_ver", it.coreVer);
    if(strlen(it.coreMd5)) obj.insert("core_md5", it.coreMd5);
    if(strlen(it.coreCompileTime)) obj.insert("core_compile_time", it.coreCompileTime);
    if(strlen(it.coreReleaseTime)) obj.insert("core_release_time", it.coreReleaseTime);

    if(strlen(it.lcdVer)) obj.insert("lcd_ver", it.lcdVer);
    if(strlen(it.lcdMd5)) obj.insert("lcd_md5", it.lcdMd5);
    if(strlen(it.lcdCompileTime)) obj.insert("lcd_compile_time", it.lcdCompileTime);
    if(strlen(it.lcdReleaseTime)) obj.insert("lcd_release_time", it.lcdReleaseTime);

    if(strlen(it.startVer)) obj.insert("start_ver", it.startVer);
    if(strlen(it.startMd5)) obj.insert("start_md5", it.startMd5);
    if(strlen(it.startCompileTime)) obj.insert("start_compile_time", it.startCompileTime);
    if(strlen(it.startReleaseTime)) obj.insert("start_release_time", it.startReleaseTime);
    if(it.version) obj.insert("pdu_ver", it.version/1.0);

    QJsonArray vs;
    for(uint i=0; i<6; ++i) vs.append(it.opVers[i]);
    obj.insert("op_vers", vs);
    json.insert(key, obj);
}

void Integr_JsonBuild::devInfo(const sDevCfg &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj; double r = 1;
    obj.insert("pdu_type", "MPDU-Pro");
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
    if(strlen(it.location)) obj.insert("location", it.location);
    if(obj.size()) json.insert(key, obj);
}

void Integr_JsonBuild::devData(sDevData *it, const QString &key, QJsonObject &json)/////////////
{
    QJsonObject obj;
    it->line.size = it->line.vol.size=it->line.cur.size=it->line.pow.size=3;//////////////////////////////////////////
    ObjData(it->line, "line_item_list", obj);

    it->loop.size = 6;/////////////////////////////////////////
    it->loop.size = it->loop.vol.size=it->loop.cur.size=it->loop.pow.size=it->loop.relay.size=6;

    ObjData(it->loop, "loop_item_list", obj, 1);

    it->group.size = 8;////////////////////////////////////////
    it->group.size = it->group.vol.size=it->group.cur.size=it->group.pow.size=it->group.relay.size=8;
    ObjData(it->group, "group_item_list", obj);

    it->dual.size = 48;/////////////////////////////////////
    it->dual.size = it->dual.vol.size=it->dual.cur.size=it->dual.pow.size=it->dual.relay.size=48;
    ObjData(it->dual, "dual_item_list", obj);

    it->output.size = 48;/////////////////////////////////////
    it->output.size = it->output.vol.size=it->output.cur.size=it->output.pow.size=it->output.relay.size=48;
    ObjData(it->output, "output_item_list", obj);

    tgObjData(it->tg, "pdu_tg_data", obj);
    envData(it->env, "env_item_list", obj);
    json.insert(key, obj);
}

void Integr_JsonBuild::netAddr(const sNetAddr &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("mode", it.mode);
    obj.insert("ip", it.ip);
    obj.insert("mask", it.mask);
    obj.insert("gw", it.gw);
    obj.insert("dns", it.dns);
    //    obj.insert("mac", it.mac);
    json.insert(key, obj);
}
