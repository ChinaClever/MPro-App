#include <stdio.h>
#include <string.h>
#include "aiot_mqtt_api.h"

/* OTA初始化时分配存储空间的函数 */
int32_t pal_ota_alloc_storage()
{
/*
TODO 分配空间存储固件
*/
return 0;
}

/* 将OTA的固件从buffer烧写到flash等介质的函数 */
int32_t pal_ota_write(unsigned char *buffer, uint32_t length)
{
/*
TODO 将固件从buffer写到flash等介质上
*/
    return 0;
}

/* OTA固件烧写结束时候的回调函数 */
int32_t pal_ota_finish()
{
/*
TODO 固件烧写结束时用户要执行的动作
*/
    printf("done ota. Notify user\n");
    return 0;
}

