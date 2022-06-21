/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "json_recv.h"

Json_Recv::Json_Recv(QObject *parent)
    : QObject{parent}
{

}

Json_Recv *Json_Recv::bulid(QObject *parent)
{
    static Json_Recv* sington = NULL;
    if(sington == NULL) {
        sington = new Json_Recv(parent);
    }
    return sington;
}

QJsonValue Json_Recv::getValue(const QJsonObject &object, const QString &key)
{
    QJsonValue value;
    if (object.contains(key))  {
        value = object.value(key);
    }
    return value;
}

QString Json_Recv::getString(const QJsonObject &object, const QString &key)
{
    QString str;
    QJsonValue value = getValue(object, key);
    if(value.isString()) {
        str = value.toString();
    }

    return str;
}

double Json_Recv::getData(const QJsonObject &object, const QString &key)
{
    double ret = -1;
    QJsonValue value = getValue(object, key);
    if(value.isDouble()) {
        ret = value.toDouble();
    }

    return ret;
}

QJsonObject Json_Recv::getObject(const QJsonObject &object, const QString &key)
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

QJsonArray Json_Recv::getArray(const QJsonObject &object, const QString &key)
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


bool Json_Recv::company(const QJsonObject &object)
{
    bool ret = false;
    QString strName = getString(object, "company");
    ret = strName.contains("CLEVER");

    return ret;
}


bool Json_Recv::versionNumber(const QJsonObject &object)
{
    bool ret = true; //company(object);
    if(ret) {
        int v = getData(object, "version");
        if(v == JSON_VERSION) ret = true;
    }

    return ret;
}


bool Json_Recv::setDataItem(const QJsonObject &object)
{
    QString key = "setDataItem";
    bool ret = true; sDataItem it;
    if (object.contains(key)) {
        QJsonObject obj = getObject(object, key);
        double res = getData(obj, "soi"); if(res >= 0) it.soi = res;
        res = getData(obj, "addr"); if(res >= 0) it.addr = res;
        res = getData(obj, "type"); if(res >= 0) it.type = res;
        res = getData(obj, "topic"); if(res >= 0) it.topic = res;
        res = getData(obj, "subtopic"); if(res >= 0) it.subtopic = res;
        res = getData(obj, "id"); if(res >= 0) it.id = res;
        res = getData(obj, "value"); if(res >= 0) it.value = res;
        it.rw = 1; it.txType = DTxType::TxJson;
        emit recvSetSig(it);
    } else ret = false;

    return ret;
}

bool Json_Recv::setNumStrItem(const QJsonObject &object)
{
    QString key = "sNumStrItem";
    bool ret = true; sNumStrItem it;
    if (object.contains(key)) {
        QJsonObject obj = getObject(object, key);
        double res = getData(obj, "soi"); if(res >= 0) it.soi = res;
        res = getData(obj, "addr"); if(res >= 0) it.addr = res;
        res = getData(obj, "isDigit"); if(res >= 0) it.isDigit = res;
        res = getData(obj, "fc"); if(res >= 0) it.fc = res;
        res = getData(obj, "id"); if(res >= 0) it.id = res;
        res = getData(obj, "subtopic"); if(res >= 0) it.sub = res;
        if(it.isDigit) {res = getData(obj, "value"); if(res >= 0) it.value = res;}
        else {QString str = getString(obj, "str"); qstrcpy(it.str, str.toLatin1().data());}
        it.rw = 1; it.txType = DTxType::TxJson; emit recvNumStrSig(it);
    } else ret = false;

    return ret;
}

bool Json_Recv::analyticalData(const QJsonObject &object)
{
    bool ret = versionNumber(object);
    if(ret) {
        setDataItem(object);
        setNumStrItem(object);
    }

    return ret;
}

bool Json_Recv::recv(const QByteArray &msg)
{
    QJsonParseError jsonerror; bool ret = false;
    QJsonDocument doc = QJsonDocument::fromJson(msg, &jsonerror);
    if (!doc.isNull() && jsonerror.error == QJsonParseError::NoError) {
        if(doc.isObject()) {
            QJsonObject object = doc.object();
            ret = analyticalData(object);
        }
    }

    return ret;
}
