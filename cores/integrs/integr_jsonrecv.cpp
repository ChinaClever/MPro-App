/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_jsonrecv.h"

Integr_JsonRecv::Integr_JsonRecv(QObject *parent)
    : QObject{parent}
{

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

double Integr_JsonRecv::getData(const QJsonObject &object, const QString &key)
{
    double ret = -1;
    QJsonValue value = getValue(object, key);
    if(value.isDouble()) {
        ret = value.toDouble();
    }

    return ret;
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


bool Integr_JsonRecv::setDataItem(const QJsonObject &object)
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

bool Integr_JsonRecv::setNumStrItem(const QJsonObject &object)
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

bool Integr_JsonRecv::analyticalData(const QJsonObject &object)
{
    bool ret = versionNumber(object);
    if(ret) {
        setDataItem(object);
        setNumStrItem(object);
    }

    return ret;
}

bool Integr_JsonRecv::recv(const QByteArray &msg)
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

