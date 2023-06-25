#ifndef AIOT_LINKSDK_H
#define AIOT_LINKSDK_H
#include <QtCore>

struct sAiotCfg
{
    bool en = true;
    int time = 10;
    char *connect = nullptr;
    QString host = "iot-as-mqtt.cn-shanghai.aliyuncs.com" ;
    QString product_key = "a1i1fEXYWYV";
    QString device_name = "PDU1";
    QString device_secret = "b2ff799f727bd2287c6ac59eb13df7ad";
};


class Aiot_LinkSdk
{
    Aiot_LinkSdk();
public:
    static Aiot_LinkSdk *build();
    static sAiotCfg aiotCfg;

    bool sdkInit();
    void sdkDeinit();
    bool sendPropertyPost(const QJsonObject &json);
    bool sendPropertyBatchPost(const QJsonObject &json);
    bool sendEventPost(const QString &event_id, const QJsonObject &json);

private:
    void initAuthCfg();
    bool send_property_batch_post(char *params);
    bool send_event_post(char *event_id, char *params);
    bool send_property_post(const QString &key, const QVariant& value, const QString &module);

};

#endif // AIOT_LINKSDK_H
