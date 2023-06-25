/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "aiot_object.h"
#include <QJsonDocument>
#include <QJsonObject>

Aiot_Object::Aiot_Object()
{
    mDevData = cm::masterDev();
    mSdk = Aiot_LinkSdk::build();
}

bool Aiot_Object::startSdk()
{
    return mSdk->sdkInit();
}

void Aiot_Object::sdkDeinit()
{
    return mSdk->sdkDeinit();
}

void Aiot_Object::appendProperty(const QString &key, const QVariant& value, const QString &module)
{
    QString id = key; if(module.size()) id.insert(0, module+":");
    mRootJson.insert(id, value.toJsonValue());
}

void Aiot_Object::appendArrayProperty(uint *ptr, int size, double unit, const QString &key, const QString &module)
{
    for(int i=0; i<size; ++i) {
        double v = ptr[i] / unit;
        QString str = key + "_" + QString::number(i+1);
        appendProperty(str, v, module);
    }
}

void Aiot_Object::clearJson()
{
    QStringList keys = mRootJson.keys();
    foreach (auto key, keys) mRootJson.remove(key);
}

bool Aiot_Object::postProperty()
{
    bool ret = mSdk->sendPropertyPost(mRootJson);
    if(!ret) cout << ret;
    return ret;
}

bool Aiot_Object::postArray(uint *ptr, int size, double unit, const QString &key, const QString &module)
{
    bool res = true;
    for(int i=0; i<size; ++i) {
        double v = ptr[i] / unit;
        QString str = key + "_" + QString::number(i+1);
        bool ret = postProperty(str, v, module);
        if(!ret){ res = false;  break;}
    }
    return res;
}

bool Aiot_Object::postProperty(const QString &key, const QVariant& value, const QString &module)
{
    QJsonObject json;
    QString id = key; if(module.size()) id.insert(0, module+":");
    json.insert(id, value.toJsonValue());

    bool ret = mSdk->sendPropertyPost(json);
    if(!ret) cout << module  << key << value;
    return ret;
}

/**
 * eventPost("alarmLog", "EventList", "罗志勇");
 */
bool Aiot_Object::eventPost(const QString &event_id, const QString &key, const QVariant& value)
{
    QJsonObject json;
    json.insert(key, value.toJsonValue());
    return mSdk->sendEventPost(event_id, json);
}
