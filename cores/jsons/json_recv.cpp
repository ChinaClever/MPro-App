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


QJsonValue Json_Recv::getValue(QJsonObject &object, const QString &key)
{
    QJsonValue value;
    if (object.contains(key))  {
        value = object.value(key);
    }
    return value;
}

QString Json_Recv::getString(QJsonObject &object, const QString &key)
{
    QString str;
    QJsonValue value = getValue(object, key);
    if(value.isString()) {
        str = value.toString();
    }

    return str;
}

double Json_Recv::getData(QJsonObject &object, const QString &key)
{
    double ret = -1;
    QJsonValue value = getValue(object, key);
    if(value.isDouble()) {
        ret = value.toDouble();
    }

    return ret;
}

QJsonObject Json_Recv::getObject(QJsonObject &object, const QString &key)
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

QJsonArray Json_Recv::getArray(QJsonObject &object, const QString &key)
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


bool Json_Recv::company(QJsonObject &object)
{
    bool ret = false;
    QString strName = getString(object, "company");
    ret = strName.contains("CLEVER");

    return ret;
}


bool Json_Recv::versionNumber(QJsonObject &object)
{
    bool ret = company(object);
    if(ret) {
        int v = getData(object, "version");
        if(v == JSON_VERSION) ret = true;
    }

    return ret;
}

