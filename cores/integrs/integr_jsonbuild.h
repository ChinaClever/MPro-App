#ifndef INTEGR_JSONBUILD_H
#define INTEGR_JSONBUILD_H
#include "commons.h"
#define JSON_VERSION   1

class Integr_JsonBuild
{
    Integr_JsonBuild();
public:
    static Integr_JsonBuild *bulid();
    QByteArray getJson(uchar addr);
    QJsonObject getJsonObject(uchar addr);
    bool saveJson(const QString &name, QJsonObject &json);

private:
    void uutInfo(const sUutInfo &it, QJsonObject &json);
    void alarmUnit(int id, const sAlarmUnit &it, const QString &key, QJsonObject &json);
    void relayUnit(int id, const sRelayUnit &it, const QString &key, QJsonObject &json);
    void ObjData(const sObjData &it, const QString &key, QJsonObject &json);
    void envData(const sEnvData &it, const QString &key, QJsonObject &json);
    void tgUnit(const sTgUnit &it, const QString &key, QJsonObject &json);
    void tgObjData(const sTgObjData &it, const QString &key, QJsonObject &json);
    void devInfo(const sDevInfo &it, const QString &key, QJsonObject &json);
    void uutInfo(const sUutInfo &it, const QString &key, QJsonObject &json);
    void devData(const sDevData *it, const QString &key, QJsonObject &json);
    void netAddr(const sNetAddr &it, const QString &key, QJsonObject &json);

private:
    QJsonObject *mJson;
};

#endif // INTEGR_JSONBUILD_H
