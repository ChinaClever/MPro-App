#include <stdio.h>
#include <string.h>
#include "aiot_mqtt_api.h"

/* 获取设备的product key */
char pal_product_key[32] = {0};
char *pal_auth_get_product_key()
{
char *pk = "a1i1fEXYWYV";
memcpy(pal_product_key, pk, strlen(pk));
return pal_product_key;
}


{
return "PDU1";
}


{
return "b2ff799f727bd2287c6ac59eb13df7ad";
}