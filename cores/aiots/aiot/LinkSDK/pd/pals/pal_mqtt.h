#ifndef _PAL_MQTT_H_
#define _PAL_MQTT_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "aiot_mqtt_api.h"

void pal_mqtt_event_connected_cb(aiot_mqtt_event_t const *event, void *handle, void *userdata);
void pal_mqtt_event_disconnected_cb(aiot_mqtt_event_t const *event, void *handle, void *userdata);
int32_t pal_mqtt_sub(void);
#if defined(__cplusplus)
}
#endif

#endif /* #ifndef _PAL_MQTT_H_ */
