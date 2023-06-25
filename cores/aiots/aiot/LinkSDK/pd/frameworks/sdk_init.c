/**
 * @file sdk_init.c
 * @brief 对LinkSDK的细粒度的接口进行封装, 提供粗粒度的接口, 方便应用的开发.
 * @date 2020-5-20
 *
 * @copyright Copyright (C) 2015-2018 Alibaba Group Holding Limited
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_mqtt_api.h"
#include "../pals/sdk_params.h"
#ifdef SDK_OTA_ENABLED
#include "aiot_ota_api.h"
#include "pal_ota.h"
#endif
#ifdef SDK_DATA_MODEL_ENABLED
#include "aiot_dm_api.h"
#endif
#ifdef SDK_DAS_ENABLED
#include "aiot_das_api.h"
#endif

#include "../pals/pal_mqtt.h"
#include "../pals/pal_auth.h"
#include "sdk_init.h"
//#include "core_string.h"

/* 位于portfiles/aiot_port文件夹下的系统适配函数集合 */
extern aiot_sysdep_portfile_t g_aiot_sysdep_portfile;

/* 位于external/ali_ca_cert.c中的服务器证书 */
extern const char *ali_ca_cert;

static pthread_t g_mqtt_process_thread;
static pthread_t g_mqtt_recv_thread;
static uint8_t g_mqtt_process_thread_running = 0;
static uint8_t g_mqtt_recv_thread_running = 0;
void *g_mqtt_handle = NULL;
#ifdef SDK_OTA_ENABLED
static void *g_ota_handle = NULL;
static pthread_t g_download_thread;     /* 用于HTTP的固件下载线程 */
#endif
#ifdef SDK_DATA_MODEL_ENABLED
void *g_dm_handle = NULL;
#endif
#ifdef SDK_DAS_ENABLED
static void *g_das_handle = NULL;
#endif
//static char *g_product_key;
//char *g_device_name;
//static char *g_device_secret;

struct sDemoAuthCfg g_aiot_auth_cfg;
static char *g_product_key = g_aiot_auth_cfg.product_key;
static char *g_device_name = g_aiot_auth_cfg.device_name;
static char *g_device_secret = g_aiot_auth_cfg.device_secret;
static char *SDK_USER_MQTT_URL = g_aiot_auth_cfg.url;

/* 日志回调函数, SDK的日志会从这里输出 */
int32_t demo_state_logcb(int32_t code, char *message)
{
    /* 下载固件的时候会有大量的HTTP收包日志, 通过code筛选出来关闭 */
    if (STATE_HTTP_LOG_RECV_CONTENT != code) {
        //printf("%s", message);
    }
    return 0;
}

/* MQTT事件回调函数, 当网络连接/重连/断开时被触发, 事件定义见core/aiot_mqtt_api.h */
void demo_mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata)
{
    switch (event->type) {
        /* SDK因为用户调用了aiot_mqtt_connect()接口, 与mqtt服务器建立连接已成功 */
        case AIOT_MQTTEVT_CONNECT: {
            printf("AIOT_MQTTEVT_CONNECT\n");
            pal_mqtt_event_connected_cb(event, handle, userdata);
            /* TODO: 处理SDK建连成功, 不可以在这里调用耗时较长的阻塞函数 */
        }
        break;

        /* SDK因为网络状况被动断连后, 自动发起重连已成功 */
        case AIOT_MQTTEVT_RECONNECT: {
            printf("AIOT_MQTTEVT_RECONNECT\n");
            /* TODO: 处理SDK重连成功, 不可以在这里调用耗时较长的阻塞函数 */
        }
        break;

        /* SDK因为网络的状况而被动断开了连接, network是底层读写失败, heartbeat是没有按预期得到服务端心跳应答 */
        case AIOT_MQTTEVT_DISCONNECT: {
            char *cause = (event->data.disconnect == AIOT_MQTTDISCONNEVT_NETWORK_DISCONNECT) ? ("network disconnect") :
                          ("heartbeat disconnect");
            printf("AIOT_MQTTEVT_DISCONNECT: %s\n", cause);
            pal_mqtt_event_disconnected_cb(event, handle, userdata);
            /* TODO: 处理SDK被动断连, 不可以在这里调用耗时较长的阻塞函数 */
        }
        break;

        default: {

        }
    }
}

#ifdef X509_ENABLED
static void demo_get_device_info(const char *topic, uint16_t topic_len, const char *payload, uint32_t payload_len)
{
    const char *target_topic = "/ext/auth/identity/response";
    char *p_product_key = NULL;
    uint32_t product_key_len = 0;
    char *p_device_name = NULL;
    uint32_t device_name_len = 0;
    int32_t res = STATE_SUCCESS;

    if (topic_len != strlen(target_topic) || memcmp(topic, target_topic, topic_len) != 0) {
        return;
    }

    /* TODO: 此处为说明上的方便, 使用了SDK内部接口 core_json_value(), 这不是一个官方API, 未来有可能变化
 *
 *              用户实际使用时, 需要换成用自己设备上可用的JSON解析函数库的接口处理payload, 比如流行的 cJSON 等
 *                  */
    res = core_json_value(payload, payload_len, "productKey", strlen("productKey"), &p_product_key, &product_key_len);
    if (res < 0) {
        return;
    }
    res = core_json_value(payload, payload_len, "deviceName", strlen("deviceName"), &p_device_name, &device_name_len);
    if (res < 0) {
        return;
    }

    if (g_product_key == NULL) {
        g_product_key = malloc(product_key_len + 1);
        if (NULL == g_product_key) {
            return;
        }

        memset(g_product_key, 0, product_key_len + 1);
        memcpy(g_product_key, p_product_key, product_key_len);
    }
    if (g_device_name == NULL) {
        g_device_name = malloc(device_name_len + 1);
        if (NULL == g_device_name) {
            return;
        }

        memset(g_device_name, 0, device_name_len + 1);
        memcpy(g_device_name, p_device_name, device_name_len);
    }

    printf("device productKey: %s\r\n", g_product_key);
    printf("device deviceName: %s\r\n", g_device_name);
}
#endif

/* MQTT默认消息处理回调, 当SDK从服务器收到MQTT消息时, 且无对应用户回调处理时被调用 */
void demo_mqtt_default_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata)
{
    switch (packet->type) {
        case AIOT_MQTTRECV_HEARTBEAT_RESPONSE: {
            printf("heartbeat response\n");
            /* TODO: 处理服务器对心跳的回应, 一般不处理 */
        }
        break;

        case AIOT_MQTTRECV_SUB_ACK: {
            printf("suback, res: -0x%04X, packet id: %d, max qos: %d\n",
                   -packet->data.sub_ack.res, packet->data.sub_ack.packet_id, packet->data.sub_ack.max_qos);
            /* TODO: 处理服务器对订阅请求的回应, 一般不处理 */
        }
        break;

        case AIOT_MQTTRECV_PUB: {
            printf("pub, qos: %d, topic: %.*s\n", packet->data.pub.qos, packet->data.pub.topic_len, packet->data.pub.topic);
            printf("pub, payload: %.*s\n", packet->data.pub.payload_len, packet->data.pub.payload);
            /* TODO: 处理服务器下发的业务报文 */
#ifdef X509_ENABLED
            /* 处理云端下发的productKey和deviceName */
            demo_get_device_info(packet->data.pub.topic, packet->data.pub.topic_len,
                                 (char *)packet->data.pub.payload, packet->data.pub.payload_len);
#endif
        }
        break;

        case AIOT_MQTTRECV_PUB_ACK: {
            printf("puback, packet id: %d\n", packet->data.pub_ack.packet_id);
            /* TODO: 处理服务器对QoS1上报消息的回应, 一般不处理 */
        }
        break;

        default: {

        }
    }
}

/* 执行aiot_mqtt_process的线程, 包含心跳发送和QoS1消息重发 */
void *demo_mqtt_process_thread(void *args)
{
    int32_t res = STATE_SUCCESS;

    while (g_mqtt_process_thread_running) {
        res = aiot_mqtt_process(args);
        if (res == STATE_USER_INPUT_EXEC_DISABLED) {
            break;
        }
        #ifdef SDK_DAS_ENABLED
        aiot_das_stepping(g_das_handle);
        #endif
        sleep(1);
    }
    return NULL;
}

/* 执行aiot_mqtt_recv的线程, 包含网络自动重连和从服务器收取MQTT消息 */
void *demo_mqtt_recv_thread(void *args)
{
    int32_t res = STATE_SUCCESS;

    while (g_mqtt_recv_thread_running) {
        res = aiot_mqtt_recv(args);
        if (res < STATE_SUCCESS) {
            if (res == STATE_USER_INPUT_EXEC_DISABLED) {
                break;
            }
            sleep(1);
        }
    }
    return NULL;
}

int32_t wrapper_auth_init(char msg[DEBUG_MSG_LEN])
{
#ifdef X509_ENABLED
    g_product_key = "";
    g_device_name = "";
    g_device_secret = "";
#else
    //g_product_key = pal_auth_get_product_key();
    //g_device_name = pal_auth_get_device_name();
    //g_device_secret = pal_auth_get_device_secret();
#endif
    return 0;
}

#if SDK_DATA_MODEL_ENABLED
void sdk_call_property_set(char *params, int32_t len);
void sdk_call_service_arrive (void *dm_handle, uint64_t msg_id, char* rrpc_id, char * service_id, char *params, int params_len);

/* 用户数据接收处理回调函数 */
static void demo_dm_recv_handler(void *dm_handle, const aiot_dm_recv_t *recv, void *userdata)
{
    printf("demo_dm_recv_handler, type = %d\r\n", recv->type);

    switch (recv->type) {

        /* 属性上报, 事件上报, 获取期望属性值或者删除期望属性值的应答 */
        case AIOT_DMRECV_GENERIC_REPLY: {
            printf("msg_id = %d, code = %d, data = %.*s, message = %.*s\r\n",
                   recv->data.generic_reply.msg_id,
                   recv->data.generic_reply.code,
                   recv->data.generic_reply.data_len,
                   recv->data.generic_reply.data,
                   recv->data.generic_reply.message_len,
                   recv->data.generic_reply.message);
        }
        break;

        /* 属性设置 */
        case AIOT_DMRECV_PROPERTY_SET: {
            printf("msg_id = %ld, params = %.*s\r\n",
                   (unsigned long)recv->data.property_set.msg_id,
                   recv->data.property_set.params_len,
                   recv->data.property_set.params);
                   sdk_call_property_set(recv->data.property_set.params, recv->data.property_set.params_len);
            /* TODO: 以下代码演示如何对来自云平台的属性设置指令进行应答, 用户可取消注释查看演示效果 */
            /*
            {
                aiot_dm_msg_t msg;

                memset(&msg, 0, sizeof(aiot_dm_msg_t));
                msg.type = AIOT_DMMSG_PROPERTY_SET_REPLY;
                msg.data.property_set_reply.msg_id = recv->data.property_set.msg_id;
                msg.data.property_set_reply.code = 200;
                msg.data.property_set_reply.data = "{}";
                int32_t res = aiot_dm_send(dm_handle, &msg);
                if (res < 0) {
                    printf("aiot_dm_send failed\r\n");
                }
            }
            */
        }
        break;

        /* 异步服务调用 */
        case AIOT_DMRECV_ASYNC_SERVICE_INVOKE: {
            printf("msg_id = %ld, service_id = %s, params = %.*s\r\n",
                   (unsigned long)recv->data.async_service_invoke.msg_id,
                   recv->data.async_service_invoke.service_id,
                   recv->data.async_service_invoke.params_len,
                   recv->data.async_service_invoke.params);
            sdk_call_service_arrive(dm_handle, recv->data.async_service_invoke.msg_id, NULL, recv->data.async_service_invoke.service_id, recv->data.async_service_invoke.params, recv->data.async_service_invoke.params_len);

            /* TODO: 以下代码演示如何对来自云平台的异步服务调用进行应答, 用户可取消注释查看演示效果
             *
             * 注意: 如果用户在回调函数外进行应答, 需要自行保存msg_id, 因为回调函数入参在退出回调函数后将被SDK销毁, 不可以再访问到
             */

        }
        break;

        /* 同步服务调用 */
        case AIOT_DMRECV_SYNC_SERVICE_INVOKE: {
            printf("msg_id = %ld, rrpc_id = %s, service_id = %s, params = %.*s\r\n",
                   (unsigned long)recv->data.sync_service_invoke.msg_id,
                   recv->data.sync_service_invoke.rrpc_id,
                   recv->data.sync_service_invoke.service_id,
                   recv->data.sync_service_invoke.params_len,
                   recv->data.sync_service_invoke.params);
            sdk_call_service_arrive (dm_handle, recv->data.async_service_invoke.msg_id, recv->data.sync_service_invoke.rrpc_id, recv->data.async_service_invoke.service_id, recv->data.async_service_invoke.params, recv->data.async_service_invoke.params_len);

            /* TODO: 以下代码演示如何对来自云平台的同步服务调用进行应答, 用户可取消注释查看演示效果
             *
             * 注意: 如果用户在回调函数外进行应答, 需要自行保存msg_id和rrpc_id字符串, 因为回调函数入参在退出回调函数后将被SDK销毁, 不可以再访问到
             */

            /*
            {
                aiot_dm_msg_t msg;

                memset(&msg, 0, sizeof(aiot_dm_msg_t));
                msg.type = AIOT_DMMSG_SYNC_SERVICE_REPLY;
                msg.data.sync_service_reply.rrpc_id = recv->data.sync_service_invoke.rrpc_id;
                msg.data.sync_service_reply.msg_id = recv->data.sync_service_invoke.msg_id;
                msg.data.sync_service_reply.code = 200;
                msg.data.sync_service_reply.service_id = "SetLightSwitchTimer";
                msg.data.sync_service_reply.data = "{}";
                int32_t res = aiot_dm_send(dm_handle, &msg);
                if (res < 0) {
                    printf("aiot_dm_send failed\r\n");
                }
            }
            */
        }
        break;

        /* 下行二进制数据 */
        case AIOT_DMRECV_RAW_DATA: {
            printf("raw data len = %d\r\n", recv->data.raw_data.data_len);
            /* TODO: 以下代码演示如何发送二进制格式数据, 若使用需要有相应的数据透传脚本部署在云端 */
            /*
            {
                aiot_dm_msg_t msg;
                uint8_t raw_data[] = {0x01, 0x02};

                memset(&msg, 0, sizeof(aiot_dm_msg_t));
                msg.type = AIOT_DMMSG_RAW_DATA;
                msg.data.raw_data.data = raw_data;
                msg.data.raw_data.data_len = sizeof(raw_data);
                aiot_dm_send(dm_handle, &msg);
            }
            */
        }
        break;

        /* 二进制格式的同步服务调用, 比单纯的二进制数据消息多了个rrpc_id */
        case AIOT_DMRECV_RAW_SYNC_SERVICE_INVOKE: {
            printf("raw sync service rrpc_id = %s, data_len = %d\r\n",
                   recv->data.raw_service_invoke.rrpc_id,
                   recv->data.raw_service_invoke.data_len);
        }
        break;

        default:
            break;
    }
}

int32_t wrapper_dm_init(char msg[DEBUG_MSG_LEN])
{
    /* 创建DATA-MODEL实例 */
    void *dm_handle = aiot_dm_init();
    if (dm_handle == NULL) {
        printf("aiot_dm_init failed");
        return -3;
    }
    /* 配置MQTT实例句柄 */
    aiot_dm_setopt(dm_handle, AIOT_DMOPT_MQTT_HANDLE, g_mqtt_handle);
    /* 配置消息接收处理回调函数 */
    aiot_dm_setopt(dm_handle, AIOT_DMOPT_RECV_HANDLER, (void *)demo_dm_recv_handler);
    g_dm_handle = dm_handle;
    return STATE_SUCCESS;
}
#endif

int32_t wrapper_mqtt_init(char msg[DEBUG_MSG_LEN])
{
    int32_t     res = STATE_SUCCESS;
    void       *mqtt_handle = NULL;
    char       *url = SDK_USER_MQTT_URL; /* 阿里云平台上海站点的域名后缀 */
    char        host[100] = {0}; /* 用这个数组拼接设备连接的云平台站点全地址, 规则是 ${productKey}.iot-as-mqtt.cn-shanghai.aliyuncs.com */
    uint16_t    port = 443;      /* 无论设备是否使用TLS连接阿里云平台, 目的端口都是443 */
    aiot_sysdep_network_cred_t cred; /* 安全凭据结构体, 如果要用TLS, 这个结构体中配置CA证书等参数 */
    uint32_t    sdk_user_heartbeat_interval = SDK_USER_MQTT_HEART_BEAT_INTERVAL_MS;
    uint8_t     sdk_user_heartbeat_max_lost_num = SDK_USER_MQTT_HEART_BEAT_MAX_LOST_NUM;
    uint8_t     sdk_user_clean_session = SDK_USER_CLEAN_SESSION;
    uint8_t     sdk_user_enable_auto_reconnect = SDK_USER_ENABLE_AUTO_RECONNECT;

    /* 赋予设备三元组,作为连云认证的凭据 */
    char *product_key       =  g_product_key;
    char *device_name       =  g_device_name;
    char *device_secret     =  g_device_secret;

    /* 配置SDK的底层依赖 */
    aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile);
    /* 配置SDK的日志输出 */
    aiot_state_set_logcb(demo_state_logcb);

    /* 创建SDK的安全凭据, 用于建立TLS连接 */
    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.max_tls_fragment = 16384; /* 最大的分片长度为16K, 其它可选值还有4K, 2K, 1K, 0.5K */
    cred.sni_enabled = 1;                               /* TLS建连时, 支持Server Name Indicator */
#ifdef ENCRYPT_TLS_ENABLED
    cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;  /* 使用RSA证书校验MQTT服务端 */
    cred.x509_server_cert = ali_ca_cert;                /* 用来验证MQTT服务端的RSA根证书 */
    cred.x509_server_cert_len = strlen(ali_ca_cert);    /* 用来验证MQTT服务端的RSA根证书长度 */
#endif
#ifdef ENCRYPT_TLS_PSK_ENABLED
    cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_PSK; /* 使用PSK校验MQTT服务端 */
    port = 1883;
#endif

    /* TODO: 留意以下4行, 使用X509双向认证时, 用户对安全凭据的设置就只要增加这一部分 */
#ifdef X509_ENABLED
    char *client_cert = pal_get_x509_cert();
    char *client_private_key = pal_get_x509_private_key();
    cred.x509_client_cert = client_cert;
    cred.x509_client_cert_len = strlen(client_cert);
    cred.x509_client_privkey = client_private_key;
    cred.x509_client_privkey_len = strlen(client_private_key);
#endif

    /* 创建1个MQTT客户端实例并内部初始化默认参数 */
    mqtt_handle = aiot_mqtt_init();
    if (mqtt_handle == NULL) {
        printf("aiot_mqtt_init failed\n");
        return -1;
    }

    /* TODO: 如果以下代码不被注释, 则例程会用TCP而不是TLS连接云平台 */
    /*
    {
        memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
        cred.option = AIOT_SYSDEP_NETWORK_CRED_NONE;
    }
    */

#ifdef DAILY_ENV
    snprintf(host, 100, "%s", url);
    port = DAILY_PORT;
#elif defined USE_INSTANCE
    snprintf(host, 100, "%s", url);
    port = 443;
#else
    snprintf(host, 100, "%s.%s", product_key, url);
#endif

#ifdef X509_ENABLED
    snprintf(host, 100, "%s", "x509.itls.cn-shanghai.aliyuncs.com");
    port = 1883;
#endif

    /* 配置MQTT服务器地址 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HOST, (void *)host);
    /* 配置MQTT服务器端口 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PORT, (void *)&port);
    /* 配置设备productKey */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, (void *)product_key);
    /* 配置设备deviceName */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, (void *)device_name);
    /* 配置设备deviceSecret */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, (void *)device_secret);
    /* 配置网络连接的安全凭据, 上面已经创建好了 */
#ifndef ENCRYPT_NONE
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred);
#endif
    /* 配置MQTT默认消息接收回调函数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void *)demo_mqtt_default_recv_handler);
    /* 配置MQTT事件回调函数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void *)demo_mqtt_event_handler);
    /* 配置MQTT的心跳间隔 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HEARTBEAT_INTERVAL_MS, (void *)&sdk_user_heartbeat_interval);
    /* 配置MQTT的最大允许的心跳丢失次数 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HEARTBEAT_MAX_LOST, (void *)&sdk_user_heartbeat_max_lost_num);
    /* 配置MQTT是否clean session */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_CLEAN_SESSION, (void *)&sdk_user_clean_session);
    /* 配置MQTT是否使能mqtt自动重连 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECONN_ENABLED, (void *)&sdk_user_enable_auto_reconnect);

    /* 与服务器建立MQTT连接 */
    res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        /* 尝试建立连接失败, 销毁MQTT实例, 回收资源 */
        aiot_mqtt_deinit(&mqtt_handle);
        printf("aiot_mqtt_connect failed: -0x%04X\n", -res);
        return res;
    }

    g_mqtt_handle = mqtt_handle;
    /*自动订阅用户自定义的订阅类型的topic */
    pal_mqtt_sub();

    /* 创建一个单独的线程, 专用于执行aiot_mqtt_process, 它会自动发送心跳保活, 以及重发QoS1的未应答报文 */
    g_mqtt_process_thread_running = 1;
    res = pthread_create(&g_mqtt_process_thread, NULL, demo_mqtt_process_thread, mqtt_handle);
    if (res < 0) {
        printf("pthread_create demo_mqtt_process_thread failed: %d\n", res);
        return res;
    }

    /* 创建一个单独的线程用于执行aiot_mqtt_recv, 它会循环收取服务器下发的MQTT消息, 并在断线时自动重连 */
    g_mqtt_recv_thread_running = 1;
    res = pthread_create(&g_mqtt_recv_thread, NULL, demo_mqtt_recv_thread, mqtt_handle);
    if (res < 0) {
        printf("pthread_create demo_mqtt_recv_thread failed: %d\n", res);
        return res;
    }
    return STATE_SUCCESS;
};

#ifdef SDK_OTA_ENABLED
/* 下载收包回调, 用户调用 aiot_download_recv() 后, SDK收到数据会进入这个函数, 把下载到的数据交给用户 */
/* TODO: 一般来说, 设备升级时, 会在这个回调中, 把下载到的数据写到Flash上 */
void demo_download_recv_handler(void *handle, const aiot_download_recv_t *packet, void *userdata)
{
    uint32_t data_buffer_len = 0;
    uint32_t last_percent = 0;
    int32_t  percent = 0;

    /* 目前只支持 packet->type 为 AIOT_DLRECV_HTTPBODY 的情况 */
    if (!packet || AIOT_DLRECV_HTTPBODY != packet->type) {
        return;
    }
    percent = packet->data.percent;

    /* userdata可以存放 demo_download_recv_handler() 的不同次进入之间, 需要共享的数据 */
    /* 这里用来存放上一次进入本回调函数时, 下载的固件进度百分比 */
    if (userdata) {
        last_percent = *((uint32_t *)(userdata));
    }

    data_buffer_len = packet->data.len;

    /* 如果 percent 为负数, 说明发生了收包异常或者digest校验错误 */
    if (percent < 0) {
        printf("exception: percent = %d\r\n", percent);
        if (userdata) {
            free(userdata);
        }
        return;
    }

    if (percent <= 100) {
        pal_ota_write(packet->data.buffer, packet->data.len);
    }
    /*
     * TODO: 下载一段固件成功, 这个时候, 用户应该将
     *       起始地址为 packet->data.buffer, 长度为 packet->data.len 的内存, 保存到flash上
     *
     *       如果烧写flash失败, 还应该调用 aiot_download_report_progress(handle, -4) 将失败上报给云平台
     *       备注:协议中, 与云平台商定的错误码在 aiot_ota_protocol_errcode_t 类型中, 例如
     *           -1: 表示升级失败
     *           -2: 表示下载失败
     *           -3: 表示校验失败
     *           -4: 表示烧写失败
     *
     *       详情可见 https://help.aliyun.com/document_detail/85700.html
     */

    /* percent 入参的值为 100 时, 说明SDK已经下载固件内容全部完成 */
    if (percent == 100) {
        pal_ota_finish();
        /*
         * TODO: 这个时候, 一般用户就应该完成所有的固件烧录, 保存当前工作, 重启设备, 切换到新的固件上启动了
                 并且, 新的固件必须要以

                 aiot_ota_report_version(ota_handle, new_version);

                 这样的操作, 将升级后的新版本号(比如1.0.0升到1.1.0, 则new_version的值是"1.1.0")上报给云平台
                 云平台收到了新的版本号上报后, 才会判定升级成功, 否则会认为本次升级失败了
                 如果下载成功后升级失败, 还应该调用 aiot_download_report_progress(handle, -1) 将失败上报给云平台
         */
    }

    /* 简化输出, 只有距离上次的下载进度增加5%以上时, 才会打印进度, 并向服务器上报进度 */
    if (percent - last_percent >= 5 || percent == 100) {
        printf("download %03d%% done, +%d bytes\r\n", percent, data_buffer_len);
        aiot_download_report_progress(handle, percent);

        if (userdata) {
            *((uint32_t *)(userdata)) = percent;
        }
        if (percent == 100 && userdata) {
            free(userdata);
        }
    }
}

/* 执行aiot_download_recv的线程, 实现固件内容的请求和接收 */
void *demo_ota_download_thread(void *dl_handle)
{
    int32_t     ret = 0;

    printf("starting download thread in 2 seconds ......\r\n");
    sleep(2);

    /* 向固件服务器请求下载 */
    /*
     * TODO: 下面这样的写法, 就是以1个请求, 获取全部的固件内容
     *       设备资源比较少, 或者网络较差时, 也可以分段下载, 需要组合
     *
     *       aiot_download_setopt(dl_handle, AIOT_DLOPT_RANGE_START, ...);
     *       aiot_download_setopt(dl_handle, AIOT_DLOPT_RANGE_END, ...);
     *       aiot_download_send_request(dl_handle);
     *
     *       实现, 这种情况下, 需要把以上组合语句放置到循环中, 多次 send_request 和 recv
     *
     */
    aiot_download_send_request(dl_handle);
    pal_ota_alloc_storage();

    while (1) {
        /* 从网络收取服务器回应的固件内容 */
        ret = aiot_download_recv(dl_handle);

        /* 固件全部下载完时, aiot_download_recv() 的返回值会等于 STATE_DOWNLOAD_FINISHED, 否则是当次获取的字节数 */
        if (STATE_DOWNLOAD_FINISHED == ret) {
            printf("download completed\r\n");
            break;
        }
        if (STATE_DOWNLOAD_RENEWAL_REQUEST_SENT == ret) {
            printf("download renewal request has been sent successfully\r\n");
            continue;
        }
        if (ret <= STATE_SUCCESS) {
            printf("download failed, error code is %d, try to send renewal request\r\n", ret);
            continue;
        }
    }

    /* 下载所有固件内容完成, 销毁下载会话, 线程自行退出 */
    aiot_download_deinit(&dl_handle);
    printf("download thread exit\r\n");

    return NULL;
}

/* 用户通过 aiot_ota_setopt() 注册的OTA消息处理回调, 如果SDK收到了OTA相关的MQTT消息, 会自动识别, 调用这个回调函数 */
void demo_ota_recv_handler(void *ota_handle, aiot_ota_recv_t *ota_msg, void *userdata)
{
    switch (ota_msg->type) {
        case AIOT_OTARECV_FOTA: {
            uint32_t res = 0;
            uint16_t port = 443;
            uint32_t max_buffer_len = (512);
            aiot_sysdep_network_cred_t cred;
            void *dl_handle = NULL;
            void *last_percent = NULL;
            uint8_t sdk_user_ota_auto_download = SDK_USER_OTA_AUTO_DOWNLOAD;

            if (NULL == ota_msg->task_desc) {
                break;
            }

            dl_handle = aiot_download_init();
            if (NULL == dl_handle) {
                break;
            }
            printf("OTA target firmware version: %s, size: %u Bytes\r\n", ota_msg->task_desc->version,
                   ota_msg->task_desc->size_total);

            if (0 == sdk_user_ota_auto_download) {
                return;
            }

            memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
            cred.option = AIOT_SYSDEP_NETWORK_CRED_SVRCERT_CA;
            cred.max_tls_fragment = 16384;
            cred.x509_server_cert = ali_ca_cert;
            cred.x509_server_cert_len = strlen(ali_ca_cert);


            /* 设置下载时为TLS下载 */
            aiot_download_setopt(dl_handle, AIOT_DLOPT_NETWORK_CRED, (void *)(&cred));
            /* 设置下载时访问的服务器端口号 */
            aiot_download_setopt(dl_handle, AIOT_DLOPT_NETWORK_PORT, (void *)(&port));
            /* 设置下载的任务信息, 通过输入参数 ota_msg 中的 task_desc 成员得到, 内含下载地址, 固件大小, 固件签名等 */
            aiot_download_setopt(dl_handle, AIOT_DLOPT_TASK_DESC, (void *)(ota_msg->task_desc));
            /* 设置下载内容到达时, SDK将调用的回调函数 */
            aiot_download_setopt(dl_handle, AIOT_DLOPT_RECV_HANDLER, (void *)(demo_download_recv_handler));
            /* 设置单次下载最大的buffer长度, 每当这个长度的内存读满了后会通知用户 */
            aiot_download_setopt(dl_handle, AIOT_DLOPT_BODY_BUFFER_MAX_LEN, (void *)(&max_buffer_len));
            /* 设置 AIOT_DLOPT_RECV_HANDLER 的不同次调用之间共享的数据, 比如例程把进度存在这里 */
            last_percent = malloc(sizeof(uint32_t));
            if (NULL == last_percent) {
                aiot_download_deinit(&dl_handle);
                break;
            }
            memset(last_percent, 0, sizeof(uint32_t));
            aiot_download_setopt(dl_handle, AIOT_DLOPT_USERDATA, (void *)last_percent);

            /* 启动专用的下载线程, 去完成固件内容的下载 */
            res = pthread_create(&g_download_thread, NULL, demo_ota_download_thread, dl_handle);
            if (res != 0) {
                printf("pthread_create demo_ota_download_thread failed: %d\r\n", res);
                aiot_download_deinit(&dl_handle);
                free(last_percent);
            } else {
                /* 下载线程被设置为 detach 类型, 固件内容获取完毕后可自主退出 */
                pthread_detach(g_download_thread);
            }
            break;
        }

        default:
            break;
    }
}

int32_t wrapper_ota_init(char msg[DEBUG_MSG_LEN])
{
    /* 与MQTT例程不同的是, 这里需要增加创建OTA会话实例的语句 */
    void *ota_handle = aiot_ota_init();
    char *cur_version = NULL;
    int32_t res;
    char *sdk_user_ota_cur_version = SDK_USER_OTA_CUR_VERSION;
    uint8_t sdk_user_ota_auto_report_version = SDK_USER_OTA_AUTO_REPORT_VERSION;
    if (NULL == ota_handle) {
        return -2;
    }

    /* 用以下语句, 把OTA会话和MQTT会话关联起来 */
    aiot_ota_setopt(ota_handle, AIOT_OTAOPT_MQTT_HANDLE, g_mqtt_handle);
    /* 用以下语句, 设置OTA会话的数据接收回调, SDK收到OTA相关推送时, 会进入这个回调函数 */
    aiot_ota_setopt(ota_handle, AIOT_OTAOPT_RECV_HANDLER, demo_ota_recv_handler);

    /* 演示MQTT连接建立起来之后, 就可以上报当前设备的版本号了 */
    if (sdk_user_ota_auto_report_version) {
        cur_version = sdk_user_ota_cur_version;
        res = aiot_ota_report_version(ota_handle, cur_version);
        if (res < STATE_SUCCESS) {
            printf("aiot_ota_report_version failed: -0x%04X\r\n", -res);
            return res;
        }
    }

    g_ota_handle = ota_handle;
    return STATE_SUCCESS;
};
#endif

#ifdef SDK_DAS_ENABLED
#ifndef SDK_USER_OTA_CUR_VERSION
    #error please define the firmware version SDK_USER_OTA_CUR_VERSION
#endif
int32_t wrapper_das_init(void) {
    int32_t res = 0;
    g_das_handle = aiot_das_init();
    if (NULL == g_das_handle) {
        printf("aiot_das_init failed\n");
        return -1;
    }

    res = aiot_das_setopt(g_das_handle, AIOT_DASOPT_MQTT_HANDLE, g_mqtt_handle);
    if (res < STATE_SUCCESS) {
        printf("aiot_das_setopt AIOT_DASOPT_MQTT_HANDLE failed, res: -0x%04X\n", -res);
        aiot_das_deinit(&g_das_handle);
        return res;
    }

    res = aiot_das_setopt(g_das_handle, AIOT_DASOPT_SET_FIRMWARE_VERSION, SDK_USER_OTA_CUR_VERSION);
    if (res < STATE_SUCCESS) {
        aiot_das_deinit(&g_das_handle);
        printf("aiot_das_setopt AIOT_DASOPT_SET_FIRMWARE_VERSION failed, res: -0x%04X\n", -res);
    }

    return res;
}
#endif /* #ifdef SDK_DAS_ENABLED */

int32_t sdk_init(char msg[DEBUG_MSG_LEN])
{
    int32_t ret = STATE_SUCCESS;
    if ((STATE_SUCCESS != (ret = wrapper_auth_init(msg)))
        || (STATE_SUCCESS != (ret = wrapper_mqtt_init(msg)))
#ifdef SDK_OTA_ENABLED
        || (STATE_SUCCESS != (ret = wrapper_ota_init(msg)))
#endif
#ifdef SDK_DATA_MODEL_ENABLED
        || (STATE_SUCCESS != (ret = wrapper_dm_init(msg)))
#endif
#ifdef SDK_DAS_ENABLED
        || (STATE_SUCCESS != (ret = wrapper_das_init()))
#endif
    ) {
        printf("sdk init failed, ret is -0X%X\n", -ret);
        return ret;
    }
    printf("sdk init success\n");
    return STATE_SUCCESS;
}

int sdk_deinit()
{
    int32_t res = 0;
#ifdef SDK_OTA_ENABLED
    aiot_ota_deinit(&g_ota_handle);
#endif
#ifdef SDK_DATA_MODEL_ENABLED
    aiot_dm_deinit(&g_dm_handle);
#endif
#ifdef SDK_DAS_ENABLED
    aiot_das_deinit(&g_das_handle);
#endif

    if (1 == g_mqtt_process_thread_running) {
        g_mqtt_process_thread_running = 0;
        pthread_join(g_mqtt_process_thread, NULL);
    }

    if (1 == g_mqtt_recv_thread_running) {
        g_mqtt_recv_thread_running = 0;
        pthread_join(g_mqtt_recv_thread, NULL);
    }

    /* 销毁MQTT实例, 一般不会运行到这里 */
    res = aiot_mqtt_deinit(&g_mqtt_handle);
    if (res < STATE_SUCCESS) {
        printf("aiot_mqtt_deinit failed: -0x%04X\n", -res);
        return -1;
    }
    return res;
}

