/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_jsonrecv.h"
#include "set_core.h"

Integr_JsonRecv::Integr_JsonRecv(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<sCfgItem>("sCfgItem");
    qRegisterMetaType<sDataItem>("sDataItem");
}

Integr_JsonRecv *Integr_JsonRecv::bulid(QObject *parent)
{
    static Integr_JsonRecv* sington = NULL;
    if(sington == NULL) {
        sington = new Integr_JsonRecv(parent);
    }
    return sington;
}

QJsonValue Integr_JsonRecv::getValue(const QJsonObject &object, const QString &key)
{
    QJsonValue value;
    if (object.contains(key))  {
        value = object.value(key);
    }
    return value;
}

QString Integr_JsonRecv::getString(const QJsonObject &object, const QString &key)
{
    QString str;
    QJsonValue value = getValue(object, key);
    if(value.isString()) {
        str = value.toString();
    }

    return str;
}

QString Integr_JsonRecv::getString(const QByteArray &msg, const QString &key)
{
    QJsonObject object; QString res;
    bool ret = checkInput(msg, object);
    if(ret) res = getString(object, key);
    return res;
}

double Integr_JsonRecv::getData(const QJsonObject &object, const QString &key)
{
    double ret = -255;
    QJsonValue value = getValue(object, key);
    if(value.isDouble()) {
        ret = value.toDouble();
    }

    return ret;
}

double Integr_JsonRecv::getData(const QByteArray &msg, const QString &key)
{
    QJsonObject object; double res= -1;
    bool ret = checkInput(msg, object);
    if(ret) res = getData(object, key);
    return res;
}

QJsonObject Integr_JsonRecv::getObject(const QJsonObject &object, const QString &key)
{
    QJsonObject obj;
    if (object.contains(key)){
        QJsonValue value = object.value(key);
        if (value.isObject()){
            obj = value.toObject();
        }
    }
    return obj;
}

QJsonObject Integr_JsonRecv::getObject(const QByteArray &msg, const QString &key)
{
    QJsonObject object;
    bool ret = checkInput(msg, object);
    if(ret) object = getObject(object, key);
    return object;
}

QJsonArray Integr_JsonRecv::getArray(const QJsonObject &object, const QString &key)
{
    QJsonArray array;
    if (object.contains(key)) {
        QJsonValue value = object.value(key);
        if (value.isArray()) {
            array = value.toArray();
        }
    }

    return array;
}


bool Integr_JsonRecv::company(const QJsonObject &object)
{
    bool ret = false;
    QString strName = getString(object, "company");
    ret = strName.contains("CLEVER");

    return ret;
}


bool Integr_JsonRecv::versionNumber(const QJsonObject &object)
{
    bool ret = true; //company(object);
    if(ret) {
        int v = getData(object, "version");
        if(v == JSON_VERSION) ret = true;
    }

    return ret;
}

bool Integr_JsonRecv::dataItem(const QString key, const QJsonObject &object, sDataItem &it)
{
    bool ret = true;
    if (object.contains(key)) {
        QJsonObject obj = getObject(object, key);
        double res = getData(obj, "addr"); if(res >= 0) it.addr = res;
        res = getData(obj, "type"); if(res >= 0) it.type = res;
        res = getData(obj, "topic"); if(res >= 0) it.topic = res;
        res = getData(obj, "subtopic"); if(res >= 0) it.subtopic = res;
        res = getData(obj, "id"); if(res >= 0) it.id = res;
        res = getData(obj, "value"); if(res >= -40){res *= cm::decimal(it);
            if((DType::Env == it.type) && (DTopic::Tem==it.topic))
                if(it.subtopic > DSub::Size && it.subtopic < DSub::EnAlarm) res += 400;
            it.value = res;} it.txType = DTxType::TxJson;
    } else ret = false;
    return ret;
}

bool Integr_JsonRecv::pduDataSet(const QJsonObject &object)
{
    QString key = "pduDataSet"; sDataItem it;
    bool ret = dataItem(key, object, it);
    if (ret) {
        it.rw = 1;
        emit recvSetSig(it);
    } else ret = false;

    return ret;
}

double Integr_JsonRecv::pduDataGet(const QJsonObject &object)
{
    QString key = "pduDataGet"; sDataItem it;
    if (dataItem(key, object, it)) {
        if(it.id) it.id -= 1;
        Set_Core::bulid()->upMetaData(it);
    }

    return it.value/cm::decimal(it);
}

bool Integr_JsonRecv::cfgItem(const QString key, const QJsonObject &object, sCfgItem &it)
{
    bool ret = true;
    if (object.contains(key)) {
        QJsonObject obj = getObject(object, key);
        double res = getData(obj, "addr"); if(res >= 0) it.addr = res;
        res = getData(obj, "type"); if(res >= 0) it.type = res;
        res = getData(obj, "fc"); if(res >= 0) it.fc = res;
        res = getData(obj, "id"); if(res >= 0) it.id = res;
        it.txType = DTxType::TxJson;
    }else ret = false;
    return ret;
}


bool Integr_JsonRecv::pduCfgSet(const QJsonObject &object)
{
    QString key = "pduCfgSet"; sCfgItem it;
    bool ret = cfgItem(key, object, it);
    if (ret) {
        QJsonObject obj = getObject(object, key);
        QVariant value = getValue(obj, "value").toVariant();
        emit recvCfgSig(it, value);
    }

    return ret;
}

bool Integr_JsonRecv::pduRelayCtrl(const QJsonObject &object)
{
    sDataItem it; bool ret = true;
    QString key = "pduRelaysCtrl";
    it.topic = DTopic::Relay;
    it.subtopic = DSub::Relays;

    if (object.contains(key)) {
        QJsonObject obj = getObject(object, key);
        double res = getData(obj, "addr"); if(res >= 0) it.addr = res;
        res = getData(obj, "start"); if(res >= 0) it.type = res;
        res = getData(obj, "num"); if(res >= 0) it.id = res;
        it.value =  getValue(obj, "on").toInt();
        it.txType = DTxType::TxJson;
        emit recvSetSig(it);
    }else ret = false;

    return ret;
}

QVariant Integr_JsonRecv::pduCfgGet(const QJsonObject &object)
{
    QString key = "pduCfgGet";
    QVariant res; sCfgItem it;
    if(cfgItem(key, object, it)) {
        res = Set_Core::bulid()->getCfg(it);
    }
    return res;
}

bool Integr_JsonRecv::analyticalData(const QJsonObject &object)
{
    //bool ret = versionNumber(object); //if(ret) {
    bool ret = pduDataSet(object);
    if(!ret) ret = pduCfgSet(object);
    if(!ret) ret = pduRelayCtrl(object);

    return ret;
}

bool Integr_JsonRecv::checkInput(const QByteArray &msg, QJsonObject &obj)
{
    QJsonParseError jsonerror; bool ret = false;
    QJsonDocument doc = QJsonDocument::fromJson(msg, &jsonerror);
    if (!doc.isNull() && jsonerror.error == QJsonParseError::NoError) {
        if(doc.isObject()) {
            obj = doc.object();
            ret = true;
        }
    } else cout << msg;
    return ret;
}

bool Integr_JsonRecv::recv(const QByteArray &msg)
{
    QJsonObject object;
    bool ret = checkInput(msg, object);
    if(ret) ret = analyticalData(object);

    return ret;
}

QVariant Integr_JsonRecv::reply(const QByteArray &msg)
{
    QJsonObject object; QVariant res;
    bool ret = checkInput(msg, object);
    if(ret) {
        QString key = "pduDataGet";
        if (object.contains(key)) res = pduDataGet(object);
        else res = pduCfgGet(object);
    }
    return res;
}
