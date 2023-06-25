#include <stdint.h>
#include <stdio.h>
#include "pal_auth.h"
#include "aiot_mqtt_api.h"
#include <string.h>

extern void *g_mqtt_handle;

int32_t pal_user_topic_pub_user_update(uint8_t *payload, uint32_t payload_len, uint8_t qos)
{
    char my_topic[200] = {0};
    int32_t res;

    snprintf(my_topic, 200, "/a1i1fEXYWYV/%s/user/update", pal_auth_get_device_name());
    res = aiot_mqtt_pub(g_mqtt_handle, my_topic, payload, payload_len, qos);
    return res;
}
int32_t pal_user_topic_pub_user_update_error(uint8_t *payload, uint32_t payload_len, uint8_t qos)
{
    char my_topic[200] = {0};
    int32_t res;

    snprintf(my_topic, 200, "/a1i1fEXYWYV/%s/user/update/error", pal_auth_get_device_name());
    res = aiot_mqtt_pub(g_mqtt_handle, my_topic, payload, payload_len, qos);
    return res;
}
void pal_mqtt_recv_user_get_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata)
{
    printf("aiot_mqtt_recv_user_get_handler\r\n");
    printf("qos: %d, topic: %.*s\n", packet->data.pub.qos, packet->data.pub.topic_len, packet->data.pub.topic);
}

int32_t pal_mqtt_sub(void)
{
    {
        char sub_topic[200] = {0};
        int32_t res;

        snprintf(sub_topic, 200, "/a1i1fEXYWYV/%s/user/get", pal_auth_get_device_name());
        res = aiot_mqtt_sub(g_mqtt_handle, sub_topic, pal_mqtt_recv_user_get_handler, 1, NULL);
        if (res < 0) {
            printf("aiot_mqtt_sub failed, res: -0x%04X\n", -res);
            return -1;
        }
    }
    return 0;
}