#include "json_build.h"
#define JSON_VERSION   1

Json_Build::Json_Build()
{
    mJson = new QJsonObject();
}


QByteArray Json_Build::getJson(uchar addr)
{
    QByteArray array;
    sDevData *dev = cm::devData(addr);  QJsonObject json;
    if(!addr) netAddr(cm::dataPacket()->net, "net_addr", json);
    if(dev->offLine > 0) {
        json.insert("company", "CLEVER");
        json.insert("version", JSON_VERSION);
        devData(dev, "pdu_data", json);
        //'saveJson("cc", json);

        QJsonDocument doc(json);
        array = doc.toJson(QJsonDocument::Compact);
    } else {
    }

    return array;
}

bool Json_Build::saveJson(const QString &name, QJsonObject &json)
{
    QFile file("F:/" + name+".json");
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


void Json_Build::uutInfo(const sUutInfo &it, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("room", it.room);
    obj.insert("cabinet", it.cab);
    obj.insert("road", it.road);
    json.insert("uut_info", QJsonValue(obj));
}

void Json_Build::alarmUnit(int id, const sAlarmUnit &it, const QString &key, QJsonObject &json)
{
    if(!it.size) return;
    QJsonObject obj; double r = 10.0;
    obj.insert("value", it.value[id]/r);
    obj.insert("rated", it.rated[id]/r);

    obj.insert("min", it.min[id]/r);
    obj.insert("max", it.max[id]/r);

    obj.insert("near_min", it.crMin[id]/r);
    obj.insert("near_max", it.crMax[id]/r);
    obj.insert("alarm", it.alarm[id]?true:false);

    json.insert(key, QJsonValue(obj));
}

void Json_Build::relayUnit(int id, const sRelayUnit &it, const QString &key, QJsonObject &json)
{
    if(!it.size) return;
    QJsonObject obj; int r = 1;
    obj.insert("state", it.sw[id]/r);
    obj.insert("mode", it.mode[id]/r);
    obj.insert("alarm", it.alarm[id]?true:false);
    obj.insert("delay", it.delay[id]/r);
    json.insert(key, QJsonValue(obj));
}

void Json_Build::ObjData(const sObjData &it, const QString &key, QJsonObject &json)
{
    QJsonArray array;
    for(int id=0; id< it.size; ++id) {
        QJsonObject obj;
        alarmUnit(id, it.vol, "vol", obj);
        alarmUnit(id, it.cur, "cur", obj);
        alarmUnit(id, it.pow, "pow", obj);
        relayUnit(id, it.relay, "relay", obj);

        double r = 10.0;
        obj.insert("id", id+1);
        obj.insert("pf", it.pf[id]/r);
        obj.insert("ele", it.ele[id]/r);
        obj.insert("name", it.name[id]);
        obj.insert("apparent_pow", it.artPow[id]/r);
        obj.insert("active_pow", it.reactivePow[id]/r);
        array.append(obj);
    }
    json.insert(key, QJsonValue(array));
}

void Json_Build::tgUnit(const sTgUnit &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj; double r = 1;
    obj.insert("value", it.value/r);
    obj.insert("rated", it.rated/r);
    obj.insert("min", it.min/r);
    obj.insert("max", it.max/r);
    obj.insert("near_emin", it.crMin/r);
    obj.insert("near_max", it.crMax/r);

    json.insert(key, QJsonValue(obj));
}

void Json_Build::tgObjData(const sTgObjData &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj; double r = 1;
    tgUnit(it.vol, "vol", obj);
    tgUnit(it.cur, "cur", obj);
    tgUnit(it.pow, "pow", obj);

    obj.insert("ele", it.ele/r);
    obj.insert("pf", it.pf/r);
    obj.insert("art", it.artPow/r);
    json.insert(key, QJsonValue(obj));
}

void Json_Build::envData(const sEnvData &it, const QString &key, QJsonObject &json)
{
    QJsonArray array;
    for(int i=0; i< it.size; ++i) {
        QJsonObject obj;
        obj.insert("id", i+1);
        alarmUnit(i, it.tem, "tem", obj);
        alarmUnit(i, it.hum, "hum", obj);
        array.append(obj);
    }
    json.insert(key, QJsonValue(array));

    QJsonArray door;
    for(int i=0; i<SENOR_NUM; ++i) door.append(it.door[i]);
    json.insert("door", door);

    QJsonArray water;
    for(int i=0; i<SENOR_NUM; ++i) door.append(it.water[i]);
    json.insert("water", water);

    QJsonArray smoke;
    for(int i=0; i<SENOR_NUM; ++i) door.append(it.smoke[i]);
    json.insert("smoke", smoke);
}

void Json_Build::devInfo(const sDevInfo &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("phases", it.phases);
    obj.insert("version", it.version);
    obj.insert("dev_name", it.devName);

    obj.insert("slave_num", it.slaveNum);
    obj.insert("op_num", it.opNum);
    obj.insert("output_num", it.outputs);

    QJsonArray ops;
    for(int i=0; i<it.opNum; ++i) ops.append(it.ops[i]);
    obj.insert("ops_num", ops);

    QJsonArray vs;
    for(int i=0; i<it.opNum; ++i) ops.append(it.opVers[i]);
    obj.insert("op_vers", vs);

    QJsonArray hz;
    for(int i=0; i<it.opNum; ++i) ops.append(it.hz[i]);
    obj.insert("hzs", hz);

    json.insert(key, QJsonValue(obj));
}


void Json_Build::uutInfo(const sUutInfo &it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("room", it.room);
    obj.insert("cabinet", it.cab);
    obj.insert("module", it.module);
    obj.insert("road", it.road);
    json.insert(key, QJsonValue(obj));
}

void Json_Build::devData(const sDevData *it, const QString &key, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("id", it->id);
    obj.insert("alarm", it->alarm?true:false);

    devInfo(it->info, "pdu_info", obj);
    uutInfo(it->uut, "uut_info", obj);

    ObjData(it->line, "line_item_list", obj);
    ObjData(it->loop, "loop_item_list", obj);
    ObjData(it->output, "output_item_list", obj);

    tgObjData(it->tg, "pdu_tg_data", obj);
    envData(it->env, "env_item_list", obj);
    obj.insert("hz", it->hz);
    json.insert(key, QJsonValue(obj));
}

void Json_Build::netAddr(const sNetAddr &it, const QString &key, QJsonObject &json)
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












