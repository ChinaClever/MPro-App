#ifndef AIOT_OBJECT_H
#define AIOT_OBJECT_H
#include "commons.h"
#include "LinkSDK/aiot_linksdk.h"



class Aiot_Object
{
public:
    Aiot_Object();
    bool startSdk();
    void sdkDeinit();

    void clearJson();
    bool eventPost(const QString &event_id, const QString &key, const QVariant& value);
    void appendProperty(const QString &key, const QVariant& value, const QString &module);
    void appendArrayProperty(uint *ptr, int size, double unit, const QString &key, const QString &module);
    bool postProperty();

    bool postProperty(const QString &key, const QVariant& value, const QString &module);
    bool postArrayProperty(const QString &key, const QList<double> &value, const QString &module);
    bool postArray(uint *ptr, int size, double unit, const QString &key, const QString &module);

protected:
    Aiot_LinkSdk *mSdk;
    sDevData *mDevData;
    QJsonObject mRootJson;
};

#endif // AIOT_OBJECT_H
