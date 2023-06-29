#include <stdio.h>
#include <string.h>
#include "aiot_mqtt_api.h"
#include "../frameworks/sdk_init.h"
extern struct sDemoAuthCfg g_aiot_auth_cfg;

/* MQTT建联成功时候的回调函数 */
void pal_mqtt_event_connected_cb(aiot_mqtt_event_t const *event, void *handle, void *userdata)
{
    /* TODO: 处理SDK建连成功, 不可以在这里调用耗时较长的阻塞函数 */
    printf("Device Connected\n");
    g_aiot_auth_cfg.connect = 1;
}

/* MQTT连接断开时候的回调函数 */
void pal_mqtt_event_disconnected_cb(aiot_mqtt_event_t const *event, void *handle, void *userdata)
{
    /* TODO: 处理SDK被动断连, 不可以在这里调用耗时较长的阻塞函数 */
    printf("Device Disconnected\n");
    g_aiot_auth_cfg.connect = 0;
}


