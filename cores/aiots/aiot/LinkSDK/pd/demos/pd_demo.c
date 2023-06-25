/**
 * @file pd_demo.c
 * @brief SDK的demo框架
 * @date 2020-5-20
 *
 * @copyright Copyright (C) 2015-2018 Alibaba Group Holding Limited
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "sdk_init.h"
#include "aiot_state_api.h"

int main()
{
    char msg[128] = {0};
    int32_t ret = STATE_SUCCESS;

    ret = sdk_init(msg);
    if (STATE_SUCCESS != ret) {
        printf("sdk init failed, ret is -0X%X\n", -ret);
        sdk_deinit();
        return -1;
    }

    while (1) {
        sleep(5);
    }

    sdk_deinit();
}
