#include <stdio.h>
#include <string.h>
#include "aiot_dm_api.h"

#include "pal_prop_post_api.h"


/**
 * @brief 上报属性runningState到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_runningState(void *dm_handle, int32_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"runningState\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}


/**
 * @brief 上报属性LineParameters:lineVol_1到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_lineVol_1(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LineParameters:lineVol_1\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LineParameters:lineVol_2到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_lineVol_2(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LineParameters:lineVol_2\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LineParameters:lineVol_3到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_lineVol_3(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LineParameters:lineVol_3\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LineParameters:linePow_1到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_linePow_1(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LineParameters:linePow_1\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LineParameters:linePow_2到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_linePow_2(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LineParameters:linePow_2\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LineParameters:linePow_3到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_linePow_3(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LineParameters:linePow_3\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性InputTotal:totalActivePower到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_totalActivePower(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"InputTotal:totalActivePower\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性InputTotal:totalApparentPower到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_totalApparentPower(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"InputTotal:totalApparentPower\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性InputTotal:totalPowerFactor到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_totalPowerFactor(void *dm_handle, int32_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"InputTotal:totalPowerFactor\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性InputTotal:totalActiveEnergy到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_totalActiveEnergy(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"InputTotal:totalActiveEnergy\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:Breaker_1到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_Breaker_1(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:Breaker_1\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:Breaker_2到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_Breaker_2(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:Breaker_2\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:Breaker_3到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_Breaker_3(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:Breaker_3\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:Breaker_4到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_Breaker_4(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:Breaker_4\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:Breaker_5到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_Breaker_5(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:Breaker_5\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:Breaker_6到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_Breaker_6(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:Breaker_6\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:loopActivePower_1到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_loopActivePower_1(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:loopActivePower_1\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:loopActivePower_2到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_loopActivePower_2(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:loopActivePower_2\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:loopActivePower_3到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_loopActivePower_3(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:loopActivePower_3\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:loopActivePower_4到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_loopActivePower_4(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:loopActivePower_4\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:loopActivePower_5到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_loopActivePower_5(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:loopActivePower_5\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性LoopParameters:loopActivePower_6到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_loopActivePower_6(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"LoopParameters:loopActivePower_6\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_1到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_1(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_1\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_2到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_2(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_2\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_3到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_3(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_3\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_4到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_4(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_4\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_5到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_5(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_5\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_6到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_6(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_6\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_7到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_7(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_7\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_8到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_8(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_8\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_9到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_9(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_9\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_10到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_10(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_10\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_11到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_11(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_11\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_12到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_12(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_12\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_13到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_13(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_13\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_14到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_14(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_14\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_15到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_15(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_15\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_16到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_16(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_16\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_17到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_17(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_17\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_18到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_18(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_18\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_19到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_19(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_19\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_20到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_20(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_20\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_21到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_21(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_21\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_22到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_22(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_22\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_23到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_23(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_23\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_24到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_24(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_24\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_25到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_25(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_25\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_26到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_26(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_26\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_27到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_27(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_27\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_28到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_28(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_28\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_29到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_29(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_29\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_30到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_30(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_30\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_31到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_31(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_31\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_32到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_32(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_32\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_33到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_33(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_33\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_34到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_34(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_34\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_35到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_35(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_35\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_36到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_36(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_36\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_37到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_37(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_37\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_38到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_38(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_38\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_39到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_39(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_39\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_40到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_40(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_40\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_41到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_41(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_41\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_42到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_42(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_42\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_43到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_43(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_43\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_44到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_44(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_44\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_45到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_45(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_45\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_46到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_46(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_46\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_47到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_47(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_47\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletSwitch_48到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletSwitch_48(void *dm_handle, uint8_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletSwitch_48\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_1到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_1(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_1\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_2到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_2(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_2\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_3到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_3(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_3\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_4到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_4(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_4\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_5到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_5(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_5\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_6到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_6(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_6\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_7到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_7(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_7\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_8到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_8(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_8\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_9到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_9(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_9\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_10到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_10(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_10\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_11到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_11(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_11\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_12到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_12(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_12\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_13到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_13(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_13\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_14到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_14(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_14\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_15到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_15(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_15\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_16到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_16(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_16\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_17到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_17(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_17\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_18到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_18(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_18\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_19到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_19(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_19\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_20到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_20(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_20\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_21到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_21(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_21\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_22到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_22(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_22\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_23到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_23(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_23\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_24到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_24(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_24\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_25到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_25(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_25\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_26到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_26(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_26\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_27到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_27(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_27\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_28到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_28(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_28\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_29到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_29(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_29\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_30到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_30(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_30\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_31到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_31(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_31\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_32到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_32(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_32\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_33到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_33(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_33\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_34到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_34(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_34\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_35到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_35(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_35\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_36到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_36(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_36\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_37到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_37(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_37\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_38到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_38(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_38\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_39到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_39(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_39\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_40到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_40(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_40\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_41到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_41(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_41\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_42到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_42(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_42\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_43到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_43(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_43\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_44到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_44(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_44\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_45到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_45(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_45\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_46到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_46(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_46\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_47到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_47(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_47\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletEle_48到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletEle_48(void *dm_handle, double value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[384] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletEle_48\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_1到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_1(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_1\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_2到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_2(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_2\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_3到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_3(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_3\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_4到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_4(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_4\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_5到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_5(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_5\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_6到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_6(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_6\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_7到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_7(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_7\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_8到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_8(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_8\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_9到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_9(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_9\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_10到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_10(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_10\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_11到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_11(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_11\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_12到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_12(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_12\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_13到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_13(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_13\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_14到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_14(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_14\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_15到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_15(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_15\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_16到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_16(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_16\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_17到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_17(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_17\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_18到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_18(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_18\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_19到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_19(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_19\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_20到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_20(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_20\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_21到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_21(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_21\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_22到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_22(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_22\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_23到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_23(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_23\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_24到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_24(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_24\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_25到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_25(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_25\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_26到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_26(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_26\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_27到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_27(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_27\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_28到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_28(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_28\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_29到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_29(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_29\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_30到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_30(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_30\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_31到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_31(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_31\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_32到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_32(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_32\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_33到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_33(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_33\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_34到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_34(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_34\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_35到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_35(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_35\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_36到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_36(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_36\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_37到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_37(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_37\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_38到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_38(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_38\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_39到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_39(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_39\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_40到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_40(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_40\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_41到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_41(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_41\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_42到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_42(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_42\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_43到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_43(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_43\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_44到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_44(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_44\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_45到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_45(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_45\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_46到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_46(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_46\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_47到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_47(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_47\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性OutletParameters:outletPow_48到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletPow_48(void *dm_handle, float value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[128] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"OutletParameters:outletPow_48\": %f}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性DevCfg:lineNum到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_lineNum(void *dm_handle, int32_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"DevCfg:lineNum\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性DevCfg:loopNum到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_loopNum(void *dm_handle, int32_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"DevCfg:loopNum\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

/**
 * @brief 上报属性DevCfg:outletNum到云端
 * @param dm_handle，dm句柄, 数据类型void *
 * @return 消息id:(>=1), 上报失败: <0
 */

int32_t pal_post_property_outletNum(void *dm_handle, int32_t value)
{
    aiot_dm_msg_t msg;
    int32_t res;
    /* TODO: 用户可以在此加入业务逻辑处理代码 */

    char property_payload[64] = {0};

    res = snprintf(property_payload, sizeof(property_payload), "{\"DevCfg:outletNum\": %d}", value);
    if (res < 0) {
        return -1;
    }

    memset(&msg, 0, sizeof(aiot_dm_msg_t));
    msg.type = AIOT_DMMSG_PROPERTY_POST;
    msg.data.property_post.params =  property_payload;
    
    return aiot_dm_send(dm_handle, &msg);
}

