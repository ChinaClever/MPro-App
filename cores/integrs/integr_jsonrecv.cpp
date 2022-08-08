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

QString Integr_JsonRecv::getString(const QByteArray &msg, const QString &key)
{
    QJsonObject object; QString res;
    bool ret = checkInput(msg, object);
    if(ret) res = getString(object, key);
    return res;
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

bool Integr_JsonRecv::getDecimal(sDataItem &it)
{
    bool ret = true;
    double res = 1;
    switch (it.topic) {
    case DTopic::Vol: res = COM_RATE_VOL; break;
    case DTopic::Cur: res = COM_RATE_CUR; break;
    case DTopic::Pow: res = COM_RATE_POW; break;
    case DTopic::Tem: res = COM_RATE_TEM; break;
    case DTopic::Hum: res = COM_RATE_HUM; break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    } it.value *= res;

    return ret;
}

bool Integr_JsonRecv::setDataItem(const QJsonObject &object)
{
    QString key = "setDataItem";
    bool ret = true; sDataItem it;
    if (object.contains(key)) {
        QJsonObject obj = getObject(object, key);
        double res = getData(obj, "addr"); if(res >= 0) it.addr = res;
        res = getData(obj, "type"); if(res >= 0) it.type = res;
        res = getData(obj, "topic"); if(res >= 0) it.topic = res;
        res = getData(obj, "subtopic"); if(res >= 0) it.subtopic = res;
        res = getData(obj, "id"); if(res >= 0) it.id = res;
        res = getData(obj, "value"); if(res >= 0) it.value = res;
        it.rw = 1; it.txType = DTxType::TxJson; getDecimal(it);
        emit recvSetSig(it);
    } else ret = false;

    return ret;
}

bool Integr_JsonRecv::setCfgItem(const QJsonObject &object)
{
    QString key = "setCfgItem";
    bool ret = true; sCfgItem it;
    if (object.contains(key)) {
        QJsonObject obj = getObject(object, key);
        double res = getData(obj, "addr"); if(res >= 0) it.addr = res;
        res = getData(obj, "type"); if(res >= 0) it.type = res;
        res = getData(obj, "fc"); if(res >= 0) it.fc = res;
        res = getData(obj, "sub"); if(res >= 0) it.sub = res;
        QVariant value = getValue(obj, "value");
        it.txType = DTxType::TxJson;
        emit recvCfgSig(it, value);
    } else ret = false;

    return ret;
}

bool Integr_JsonRecv::analyticalData(const QJsonObject &object)
{
    bool ret = versionNumber(object);
    if(ret) {
        setDataItem(object);
        setCfgItem(object);
    }

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
    }
    return ret;
}

bool Integr_JsonRecv::recv(const QByteArray &msg)
{
    QJsonObject object;
    bool ret = checkInput(msg, object);
    if(ret) ret = analyticalData(object);

    return ret;
}

