#ifndef _SDK_INIT_H_
#define _SDK_INIT_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include "aiot_state_api.h"

#define DEBUG_MSG_LEN (128)

struct sDemoAuthCfg
{
    int connect;
    char url[256];
    char product_key[256];
    char device_name[256];
    char device_secret[256];
};

int sdk_init(char msg[DEBUG_MSG_LEN]);

int sdk_deinit();

#if defined(__cplusplus)
}
#endif

#endif /* #ifndef _SDK_INIT_H_ */
