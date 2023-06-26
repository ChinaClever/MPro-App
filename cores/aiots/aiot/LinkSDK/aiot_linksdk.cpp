/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "aiot_linksdk.h"
#include "pd/pals/pal_prop_post_api.h"
#include "pd/frameworks/sdk_init.h"
#include "aiot_state_api.h"
extern void *g_dm_handle;
extern struct sDemoAuthCfg g_aiot_auth_cfg;

sAiotCfg Aiot_LinkSdk::aiotCfg;
Aiot_LinkSdk::Aiot_LinkSdk()
{
    sDemoAuthCfg *cfg = &g_aiot_auth_cfg;
    aiotCfg.connect = &(cfg->connect);
    cfg->connect = 0;
}

Aiot_LinkSdk *Aiot_LinkSdk::build()
{
    static Aiot_LinkSdk* sington = nullptr;
    if(sington == nullptr) {
        sington = new Aiot_LinkSdk();
    }
    return sington;
}

void Aiot_LinkSdk::initAuthCfg()
{
    sDemoAuthCfg *cfg = &g_aiot_auth_cfg;
    aiotCfg.connect = &(cfg->connect);
    qstrcpy(cfg->url, aiotCfg.host.toLatin1().data());
    qstrcpy(cfg->product_key, aiotCfg.product_key.toLatin1().data());
    qstrcpy(cfg->device_name, aiotCfg.device_name.toLatin1().data());
    qstrcpy(cfg->device_secret, aiotCfg.device_secret.toLatin1().data());
}

void Aiot_LinkSdk::sdkDeinit()
{
    g_aiot_auth_cfg.connect = 0;
    if(g_dm_handle) sdk_deinit();
    g_dm_handle = nullptr;
}

bool Aiot_LinkSdk::sdkInit()
{
    char msg[128] = {0};
    bool res = true;

    initAuthCfg(); sdkDeinit();
    int32_t ret = sdk_init(msg);
    if (STATE_SUCCESS != ret) {
        printf("sdk init failed, ret is -0X%X\n", -ret);
        sdkDeinit(); res = false;
    }
    return res;
}

/**
 * "{\"LightSwitch\": 0}"
 */
bool Aiot_LinkSdk::send_property_post(const QString &key, const QVariant& value, const QString &module)
{
    bool ret = false;
    aiot_dm_msg_t msg; void *dm_handle = g_dm_handle;
    if(!dm_handle || !g_aiot_auth_cfg.connect) return ret;

    QString fmd = "{\"%1%2\": %3}";
    static char property_payload[512] = {0};
    QString m; if(module.size()) m = module +":";
    if(value.type() == QVariant::String) fmd = "{\"%1%2\": \"%3\"}";
    QString cmd = fmd.arg(m, key, value.toString());

    qstrcpy(property_payload, cmd.toLatin1().data());
    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    int res = aiot_dm_send(dm_handle, &msg);
    if(res >=STATE_SUCCESS) ret = true;
    return ret;
}

/**
 * 属性上报
 * "{\"LightSwitch\": 0}"
 * "{\"module:key\": 0}" 有模块名
 */
bool Aiot_LinkSdk::sendPropertyPost(const QJsonObject &json)
{
    static char property_payload[1024*50] = {0};
    bool ret = false; QJsonDocument jsonDoc(json);
    QByteArray ba = jsonDoc.toJson(QJsonDocument::Indented);

    aiot_dm_msg_t msg; void *dm_handle = g_dm_handle;
    if(!dm_handle || !g_aiot_auth_cfg.connect) return ret;
    //char *property_payload = ba.data();

    qstrcpy(property_payload, ba.data());
    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    int res = aiot_dm_send(dm_handle, &msg);
    if(res >=STATE_SUCCESS) ret = true;
    return ret;
}


/**
 * "{\"properties\":{\"Power\": [ {\"value\":\"on\",\"time\":1612684518}],
 *  \"WF\": [{\"value\": 3,\"time\":1612684518}]}}");
 *
 */
bool Aiot_LinkSdk::send_property_batch_post(char *params)
{
    bool ret = false;
    aiot_dm_msg_t msg; void *dm_handle = g_dm_handle;
    if(!dm_handle || !g_aiot_auth_cfg.connect) return ret;
    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_BATCH_POST;
    msg.data.property_post.params = params;
    int res = aiot_dm_send(dm_handle, &msg);
    if(res >=STATE_SUCCESS) ret = true;
    return ret;
}

/**
 * 批量属性上报
 *
 */
bool Aiot_LinkSdk::sendPropertyBatchPost(const QJsonObject &json)
{
    QJsonObject root; static char property_payload[1024*10] = {0};
    root.insert("properties", json); QJsonDocument jsonDoc(root);
    QByteArray ba = jsonDoc.toJson(QJsonDocument::Indented);
    qstrcpy(property_payload, ba.data());
    char *params = property_payload;
    return send_property_batch_post(params);
}

/**
 * 事件上报函数演示
 * send_event_post( "Error", "{\"ErrorCode\": 0}");
 */
bool Aiot_LinkSdk::send_event_post(char *event_id, char *params)
{
    bool ret = false;
    aiot_dm_msg_t msg; void *dm_handle = g_dm_handle;
    if(!dm_handle || !g_aiot_auth_cfg.connect) return ret;

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_EVENT_POST;
    msg.data.event_post.event_id = event_id;
    msg.data.event_post.params = params;

    int res = aiot_dm_send(dm_handle, &msg);
    if(res >=STATE_SUCCESS) ret = true;
    return ret;
}

bool Aiot_LinkSdk::sendEventPost(const QString &event_id, const QJsonObject &json)
{
    QJsonDocument jsonDoc(json); static char property_payload[512] = {0};
    QByteArray ba = jsonDoc.toJson(QJsonDocument::Indented);
    char *id = event_id.toLatin1().data();
    qstrcpy(property_payload, ba.data());
    char *params = property_payload;
    return send_event_post(id, params);
}
