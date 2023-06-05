/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_jsonbuild.h"
#include "set_core.h"

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

// dc 0 使用系统配置， 1读的参数 2 所有参数 3网页数据
QByteArray Integr_JsonBuild::getJson(uchar addr, int dc)
{
    QByteArray array; QJsonObject json = getJsonObject(addr, dc);
    if(!json.isEmpty()) {
        QJsonDocument doc(json);
        array = doc.toJson(QJsonDocument::Compact);
    }

    return array;
}

QJsonObject Integr_JsonBuild::getJsonObject(uchar addr, int dc)
{
    if(dc) mDataContent = dc;
    else mDataContent = cm::masterDev()->cfg.param.jsonContent;
    sDevData *dev = cm::devData(addr); QJsonObject json;
    if(dev->offLine > 0 || addr == 0) {
        //json.insert("company", "CLEVER");
        json.insert("addr", addr);
        devData(dev, "pdu_data", json);
        json.insert("status", dev->status);
        uutInfo(dev->cfg.uut, "uut_info", json);

        if(dc > 1) {
            online(json);
            faultCode(dev, json);
            devInfo(dev->cfg, "pdu_info", json);
            verInfo(dev->cfg.vers, "pdu_version", json);
            if(!addr) netAddr(cm::dataPacket()->net, "net_addr", json);
            if(dc > 2) loginPermit(json); //json.insert("login_permit", Set_Core::bulid()->loginPermit()?1:0);
        }
        QDateTime datetime = QDateTime::currentDateTime();
        json.insert("datetime", datetime.toString("yyyy-MM-dd hh:mm:ss"));
        json.insert("version", JSON_VERSION);
    } else {
        cout << dev->offLine;
    }
    return json;
}

void Integr_JsonBuild::saveJson(uchar addr)
{
    QJsonObject json = getJsonObject(addr, 3);
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

void Integr_JsonBuild::online(QJsonObject &json)
{
    int size = DEV_NUM/10;
    uint array[DEV_NUM] = {0};
    for(int i=0; i<size; ++i) {
        sDevData *dev = cm::devData(i);
        array[i] = dev->offLine>1?1:0;
    } arrayAppend(array, size, "online", json);
}

void Integr_JsonBuild::loginPermit(QJsonObject &json)
{
    QJsonObject obj;
    for(int i=0; i<USER_NUM; ++i) {
        sDevLogin *it = &cm::dataPacket()->login[i];
        if(strlen(it->user)) obj.insert(it->user, it->permit);
    } json.insert("login_permits", obj);
}

void Integr_JsonBuild::alarmUnit(const sAlarmUnit &it, const QString &key, QJsonObject &json, double r)
{
    int size = it.size; bool dc = false;
    arrayAppend(it.value, size, key+"_value", json, r);
    arrayAppend(it.alarm, size, key+"_alarm_status", json);

    if(mDataContent == 0){
        for(int i=0; i<size; ++i) dc |= it.alarm[i];
    } else if(mDataContent > 1) dc = true;

    if(dc) {
        arrayAppend(it.rated, size, key+"_rated", json, r);
        arrayAppend(it.en, size, key+"_alarm_enable", json);
        arrayAppend(it.min, size, key+"_alarm_min", json, r);
        arrayAppend(it.max, size, key+"_alarm_max", json, r);
        arrayAppend(it.crMin, size, key+"_warn_min", json, r);
        arrayAppend(it.crMax, size, key+"_warn_max", json, r);
        arrayAppend(it.hda, size, key+"_hda_en", json);
    }
}

void Integr_JsonBuild::strListAppend(const char (*ptr)[NAME_SIZE], int size, const QString &key, QJsonObject &json)
{    
    QJsonArray array;
    for(int i=0; i<size; ++i) {
         array.append(ptr[i]); //if(strlen(ptr[i]))
    } if(array.size()) json.insert(key, array);
}

void Integr_JsonBuild::relayUnit(const sRelayUnit &it, const QString &key, QJsonObject &json)
{
    int size = it.size; bool dc = false;
    arrayAppend(it.sw, size, key+"_state", json);

    if(mDataContent == 0) {
        for(int i=0; i<size; ++i) dc |= it.alarm[i];
    } else if(mDataContent > 1) dc = true;

    if(dc) {
        arrayAppend(it.alarm, size, key+"_alarm", json);
        arrayAppend(it.disabled, size, key+"_disabled", json);
        arrayAppend(it.offAlarm, size, key+"_off_alarm", json);
        arrayAppend(it.resetDelay, size, key+"_reset_delay", json);
        arrayAppend(it.powerUpDelay, size, key+"_powerup_delay", json);
        arrayAppend(it.overrunOff, size, key+"_overrun_off", json);
        arrayAppend(it.timingEn, size, key+"_timing_en", json);

        arrayAppend(it.cnt, size, key+"_use_cnt", json);
        arrayAppend(it.maxCnt, size, key+"_max_cnt", json);
        arrayAppend(it.lifeEn, size, key+"_life_alarm", json);
        strListAppend(it.timingOn, size, key+"_timing_on", json);
        strListAppend(it.timingOff, size, key+"_timing_off", json);
    }
}

void Integr_JsonBuild::groupRelayUnit(const sRelayUnit &it, const QString &key, QJsonObject &json)
{
    int size = it.size; bool dc = false;
    //arrayAppend(it.sw, size, key+"_state", json);
    if(mDataContent == 0) {
        for(int i=0; i<size; ++i) dc |= it.timingEn[i];
    } else if(mDataContent > 1) dc = true;

    if(dc) {
        arrayAppend(it.timingEn, size, key+"_timing_en", json);
        strListAppend(it.timingOn, size, key+"_timing_on", json);
        strListAppend(it.timingOff, size, key+"_timing_off", json);
    }
}

void Integr_JsonBuild::ObjData(const sObjData &it, const QString &key, QJsonObject &json, int type)
{
    QJsonObject obj;  int size = it.size;
    alarmUnit(it.vol, "vol", obj, COM_RATE_VOL);
    alarmUnit(it.cur, "cur", obj, COM_RATE_CUR);
    alarmUnit(it.pow, "pow", obj, COM_RATE_POW);

    arrayAppend(it.pf, size, "pf", obj, COM_RATE_PF);
    arrayAppend(it.ele, size, "ele", obj, COM_RATE_ELE);
    arrayAppend(it.artPow, size, "apparent_pow", obj, COM_RATE_POW);
    arrayAppend(it.reactivePow, size, "reactive_pow", obj, COM_RATE_POW);
    if(mDataContent > 1) arrayAppend(it.hdaEle, size, "hda_ele", obj, 1);

    if(3 == type) relayUnit(it.relay, "relay", obj);
    else if(4 == type) groupRelayUnit(it.relay, "group_relay", obj);
    else if(5 == type) groupRelayUnit(it.relay, "dual_relay", obj);
    else if(2 == type) arrayAppend(it.relay.sw, it.relay.size, "breaker", obj);
    else if(it.vol.size > 1) arrayAppend(it.lineVol, size, "phase_voltage", obj, COM_RATE_VOL);
    if(type > 2) { if(!size) {size = it.relay.size;} strListAppend(it.name, size, "name", obj); }

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
    QJsonObject obj; if(mDataContent < 3) {
        if(it.door[0]||it.door[1])arrayAppend(it.door, 2, "door", obj);
        if(it.water[0]) arrayAppend(it.water, 1, "water", obj);
        if(it.smoke[0]) arrayAppend(it.smoke, 1, "smoke", obj);
        bool ret = false; for(int i=0; i<SENOR_NUM; ++i) if(it.isInsert[i]) ret = true;
        if(ret) alarmUnit(it.tem, "tem", obj, COM_RATE_TEM);
        if(ret) alarmUnit(it.hum, "hum", obj, COM_RATE_HUM);
    } else {
        sAlarmUnit tem=it.tem, hum=it.hum;
        tem.size = hum.size = SENOR_NUM;
        arrayAppend(it.door, 2, "door", obj);
        arrayAppend(it.water, 1, "water", obj);
        arrayAppend(it.smoke, 1, "smoke", obj);
        alarmUnit(tem, "tem", obj, COM_RATE_TEM);
        alarmUnit(hum, "hum", obj, COM_RATE_HUM);
    }

    arrayAppend(it.isInsert, SENOR_NUM, "insert", obj);
    if(obj.size()) json.insert(key, obj);
}

void Integr_JsonBuild::verInfo(const sVersions &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("usr", it.usr);
    obj.insert("md5", it.md5);
    obj.insert("ver", it.ver);
    obj.insert("dev", it.dev);
    obj.insert("remark", it.remark);
    obj.insert("hwVersion", it.hwVersion);
    obj.insert("oldVersion", it.oldVersion);
    obj.insert("compileDate", it.compileDate);
    obj.insert("releaseDate", it.releaseDate);
    obj.insert("upgradeDate", it.upgradeDate);
    obj.insert("serialNumber", it.serialNumber);

    QJsonArray vs;
    for(uint i=0; i<6; ++i) vs.append(it.opVers[i]/10.0);
    obj.insert("op_vers", vs); json.insert(key, obj);
}

void Integr_JsonBuild::devInfo(const sDevCfg &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj; double r = 1;
    //obj.insert("pdu_type", "MPDU-Pro");
    obj.insert("line_num", it.nums.lineNum/r);
    obj.insert("pdu_spec", it.param.devSpec/r);
    obj.insert("dev_mode", it.param.devMode/r);
    obj.insert("language", it.param.language/r);

    obj.insert("pdu_hz", it.param.hz/r);
    obj.insert("op_num", it.nums.boardNum/r);
    obj.insert("loop_num", it.nums.loopNum/r);
    //obj.insert("slave_num", it.nums.slaveNum/r);
    obj.insert("output_num", it.nums.outputNum/r);
    obj.insert("cascade_addr", it.param.cascadeAddr/r);

    if(mDataContent > 2) {
        obj.insert("board_num", it.nums.boardNum/r);
        obj.insert("group_en", it.param.groupEn/r);
        obj.insert("sensor_box", it.param.sensorBoxEn/r);
        obj.insert("stand_neutral", it.param.standNeutral/r);
        obj.insert("web_background", it.param.webBackground/r);
        obj.insert("language", it.param.language/r);
        obj.insert("run_time", it.param.runTime/r);
        obj.insert("breaker", it.param.isBreaker/r);
        obj.insert("vh", it.param.vh/r);
    }

    int num = cm::masterDev()->cfg.nums.slaveNum;
    obj.insert("slave_num", num);

    int start=1, end=0;
    QJsonArray loopEnd, loopStart;
    for(uint i=0; i<it.nums.loopNum; ++i) {
        if(i) start += it.nums.loopEachNum[i-1];
        end += it.nums.loopEachNum[i];
        loopEnd.append(end);
        loopStart.append(start);
    }
    obj.insert("loop_ends", loopEnd);
    obj.insert("loop_start", loopStart);

    QJsonArray loops;
    for(uint i=0; i<LOOP_NUM; ++i) {
        int num = it.nums.loopEachNum[i];
        if(i<it.nums.loopNum) loops.append(num); else loops.append(0);
    } obj.insert("loop_array", loops);

    QJsonArray ops;
    for(uint i=0; i<it.nums.boardNum; ++i) ops.append(it.nums.boards[i]);
    obj.insert("ops_num", ops); json.insert(key, obj);
}


void Integr_JsonBuild::uutInfo(const sUutInfo &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj; int dc = mDataContent;
    if((dc > 1) || qstrlen(it.sn)) obj.insert("sn", it.sn);
    if((dc > 1) || qstrlen(it.room)) obj.insert("room", it.room);
    if((dc > 1) || qstrlen(it.uuid)) obj.insert("uuid", it.uuid);
    if((dc > 1) || qstrlen(it.devName)) obj.insert("name", it.devName);
    if((dc > 1) || qstrlen(it.qrcode)) obj.insert("qrcode", it.qrcode);
    if((dc > 1) || qstrlen(it.devType)) obj.insert("pdu_type", it.devType);
    if((dc > 1) || qstrlen(it.location)) obj.insert("location", it.location);
    if(obj.size()) json.insert(key, obj);
}

void Integr_JsonBuild::webGroupData(sDevData *it, QJsonObject &obj)
{
    sObjData group = it->group;
    group.size = group.pow.size = GROUP_NUM;
    group.relay.size = it->output.relay.size ? GROUP_NUM:0;
    ObjData(group, "group_item_list", obj, 4);

    sObjData dual = it->dual;
    dual.size = dual.pow.size = it->output.size;
    dual.relay.size = it->output.relay.size;
    ObjData(dual, "dual_item_list", obj, 5);
}

void Integr_JsonBuild::devData(sDevData *it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    ObjData(it->line, "line_item_list", obj, 1);
    ObjData(it->loop, "loop_item_list", obj, 2);
    ObjData(it->output, "output_item_list", obj, 3);

    if(mDataContent < 3) {
        ObjData(it->group, "group_item_list", obj, 4);
        ObjData(it->dual, "dual_item_list", obj, 5);
    } else webGroupData(it, obj);

    tgObjData(it->tg, "pdu_tg_data", obj);
    envData(it->env, "env_item_list", obj);
    json.insert(key, obj);
}

void Integr_JsonBuild::netAddr(const sNetInterface &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    const sNetAddr *inet = &(it.inet);
    obj.insert("mode", inet->dhcp);
    obj.insert("ip", inet->ip);
    obj.insert("mask", inet->mask);
    obj.insert("gw", inet->gw);
    obj.insert("dns", inet->dns);
    obj.insert("mac", it.mac);

    inet = &(it.inet6);
    if(inet->en || mDataContent > 2) {
        obj.insert("mode6", inet->dhcp);
        obj.insert("ip6", inet->ip);
        obj.insert("gw6", inet->gw);
        obj.insert("dns6", inet->dns);
        obj.insert("prefix6_len", inet->prefixLen);
    }

    json.insert(key, obj);
}
