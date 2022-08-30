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
    void strListAppend(const char (*ptr)[NAME_SIZE], int size, const QString &key, QJsonObject &json);
    void arrayAppend(const uint *ptr, int size, const QString &key, QJsonObject &json, double r=1);
    void alarmUnit(const sAlarmUnit &it, const QString &key, QJsonObject &json, double r);
    void relayUnit(const sRelayUnit &it, const QString &key, QJsonObject &json);
    void ObjData(const sObjData &it, const QString &key, QJsonObject &json, bool breaker=0);
    void envData(const sEnvData &it, const QString &key, QJsonObject &json);
    void tgUnit(const sTgUnit &it, const QString &key, QJsonObject &json, double r);
    void tgObjData(const sTgObjData &it, const QString &key, QJsonObject &json);
    void verInfo(const sVersions &it, const QString &key, QJsonObject &json);
    void devInfo(const sDevCfg &it, const QString &key, QJsonObject &json);
    void uutInfo(const sUutInfo &it, const QString &key, QJsonObject &json);
    void devData(sDevData *it, const QString &key, QJsonObject &json);
    void faultCode(sDevData *dev, QJsonObject &json);
    void netAddr(const sNetAddr &it, const QString &key, QJsonObject &json);

private:
    QJsonObject *mJson;
};

#endif // INTEGR_JSONBUILD_H
