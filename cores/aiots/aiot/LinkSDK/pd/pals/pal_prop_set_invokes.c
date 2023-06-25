/*
 * Copyright (C) 2015-2019 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <string.h>
#include "pal_prop_set_api.h"

void sdk_call_property_set(char *params, int len) {
   /**
 * @brief 上报属性LineParameters_lineVol_1到云端
 * @param value 属性LineParameters_lineVol_1的值
 */

    if(memcmp(params + 2, "LineParameters:lineVol_1", strlen("LineParameters:lineVol_1")) == 0) {
        pal_LineParameters_lineVol_1_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LineParameters_lineVol_2到云端
 * @param value 属性LineParameters_lineVol_2的值
 */

    if(memcmp(params + 2, "LineParameters:lineVol_2", strlen("LineParameters:lineVol_2")) == 0) {
        pal_LineParameters_lineVol_2_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LineParameters_lineVol_3到云端
 * @param value 属性LineParameters_lineVol_3的值
 */

    if(memcmp(params + 2, "LineParameters:lineVol_3", strlen("LineParameters:lineVol_3")) == 0) {
        pal_LineParameters_lineVol_3_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LineParameters_linePow_1到云端
 * @param value 属性LineParameters_linePow_1的值
 */

    if(memcmp(params + 2, "LineParameters:linePow_1", strlen("LineParameters:linePow_1")) == 0) {
        pal_LineParameters_linePow_1_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LineParameters_linePow_2到云端
 * @param value 属性LineParameters_linePow_2的值
 */

    if(memcmp(params + 2, "LineParameters:linePow_2", strlen("LineParameters:linePow_2")) == 0) {
        pal_LineParameters_linePow_2_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LineParameters_linePow_3到云端
 * @param value 属性LineParameters_linePow_3的值
 */

    if(memcmp(params + 2, "LineParameters:linePow_3", strlen("LineParameters:linePow_3")) == 0) {
        pal_LineParameters_linePow_3_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性InputTotal_totalActivePower到云端
 * @param value 属性InputTotal_totalActivePower的值
 */

    if(memcmp(params + 2, "InputTotal:totalActivePower", strlen("InputTotal:totalActivePower")) == 0) {
        pal_InputTotal_totalActivePower_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性InputTotal_totalApparentPower到云端
 * @param value 属性InputTotal_totalApparentPower的值
 */

    if(memcmp(params + 2, "InputTotal:totalApparentPower", strlen("InputTotal:totalApparentPower")) == 0) {
        pal_InputTotal_totalApparentPower_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性InputTotal_totalPowerFactor到云端
 * @param value 属性InputTotal_totalPowerFactor的值
 */

    if(memcmp(params + 2, "InputTotal:totalPowerFactor", strlen("InputTotal:totalPowerFactor")) == 0) {
        pal_InputTotal_totalPowerFactor_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性InputTotal_totalActiveEnergy到云端
 * @param value 属性InputTotal_totalActiveEnergy的值
 */

    if(memcmp(params + 2, "InputTotal:totalActiveEnergy", strlen("InputTotal:totalActiveEnergy")) == 0) {
        pal_InputTotal_totalActiveEnergy_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_loopBreakerStatus到云端
 * @param value 属性LoopParameters_loopBreakerStatus的值
 */

    if(memcmp(params + 2, "LoopParameters:loopBreakerStatus", strlen("LoopParameters:loopBreakerStatus")) == 0) {
        pal_LoopParameters_loopBreakerStatus_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_loopActivePowers到云端
 * @param value 属性LoopParameters_loopActivePowers的值
 */

    if(memcmp(params + 2, "LoopParameters:loopActivePowers", strlen("LoopParameters:loopActivePowers")) == 0) {
        pal_LoopParameters_loopActivePowers_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_Breaker_1到云端
 * @param value 属性LoopParameters_Breaker_1的值
 */

    if(memcmp(params + 2, "LoopParameters:Breaker_1", strlen("LoopParameters:Breaker_1")) == 0) {
        pal_LoopParameters_Breaker_1_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_Breaker_2到云端
 * @param value 属性LoopParameters_Breaker_2的值
 */

    if(memcmp(params + 2, "LoopParameters:Breaker_2", strlen("LoopParameters:Breaker_2")) == 0) {
        pal_LoopParameters_Breaker_2_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_Breaker_3到云端
 * @param value 属性LoopParameters_Breaker_3的值
 */

    if(memcmp(params + 2, "LoopParameters:Breaker_3", strlen("LoopParameters:Breaker_3")) == 0) {
        pal_LoopParameters_Breaker_3_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_Breaker_4到云端
 * @param value 属性LoopParameters_Breaker_4的值
 */

    if(memcmp(params + 2, "LoopParameters:Breaker_4", strlen("LoopParameters:Breaker_4")) == 0) {
        pal_LoopParameters_Breaker_4_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_Breaker_5到云端
 * @param value 属性LoopParameters_Breaker_5的值
 */

    if(memcmp(params + 2, "LoopParameters:Breaker_5", strlen("LoopParameters:Breaker_5")) == 0) {
        pal_LoopParameters_Breaker_5_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_Breaker_6到云端
 * @param value 属性LoopParameters_Breaker_6的值
 */

    if(memcmp(params + 2, "LoopParameters:Breaker_6", strlen("LoopParameters:Breaker_6")) == 0) {
        pal_LoopParameters_Breaker_6_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_loopActivePower_1到云端
 * @param value 属性LoopParameters_loopActivePower_1的值
 */

    if(memcmp(params + 2, "LoopParameters:loopActivePower_1", strlen("LoopParameters:loopActivePower_1")) == 0) {
        pal_LoopParameters_loopActivePower_1_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_loopActivePower_2到云端
 * @param value 属性LoopParameters_loopActivePower_2的值
 */

    if(memcmp(params + 2, "LoopParameters:loopActivePower_2", strlen("LoopParameters:loopActivePower_2")) == 0) {
        pal_LoopParameters_loopActivePower_2_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_loopActivePower_3到云端
 * @param value 属性LoopParameters_loopActivePower_3的值
 */

    if(memcmp(params + 2, "LoopParameters:loopActivePower_3", strlen("LoopParameters:loopActivePower_3")) == 0) {
        pal_LoopParameters_loopActivePower_3_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_loopActivePower_4到云端
 * @param value 属性LoopParameters_loopActivePower_4的值
 */

    if(memcmp(params + 2, "LoopParameters:loopActivePower_4", strlen("LoopParameters:loopActivePower_4")) == 0) {
        pal_LoopParameters_loopActivePower_4_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_loopActivePower_5到云端
 * @param value 属性LoopParameters_loopActivePower_5的值
 */

    if(memcmp(params + 2, "LoopParameters:loopActivePower_5", strlen("LoopParameters:loopActivePower_5")) == 0) {
        pal_LoopParameters_loopActivePower_5_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性LoopParameters_loopActivePower_6到云端
 * @param value 属性LoopParameters_loopActivePower_6的值
 */

    if(memcmp(params + 2, "LoopParameters:loopActivePower_6", strlen("LoopParameters:loopActivePower_6")) == 0) {
        pal_LoopParameters_loopActivePower_6_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletRelayStatus到云端
 * @param value 属性OutletParameters_outletRelayStatus的值
 */

    if(memcmp(params + 2, "OutletParameters:outletRelayStatus", strlen("OutletParameters:outletRelayStatus")) == 0) {
        pal_OutletParameters_outletRelayStatus_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletRealTimePowers到云端
 * @param value 属性OutletParameters_outletRealTimePowers的值
 */

    if(memcmp(params + 2, "OutletParameters:outletRealTimePowers", strlen("OutletParameters:outletRealTimePowers")) == 0) {
        pal_OutletParameters_outletRealTimePowers_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletActiveEnergys到云端
 * @param value 属性OutletParameters_outletActiveEnergys的值
 */

    if(memcmp(params + 2, "OutletParameters:outletActiveEnergys", strlen("OutletParameters:outletActiveEnergys")) == 0) {
        pal_OutletParameters_outletActiveEnergys_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_1到云端
 * @param value 属性OutletParameters_outletSwitch_1的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_1", strlen("OutletParameters:outletSwitch_1")) == 0) {
        pal_OutletParameters_outletSwitch_1_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_2到云端
 * @param value 属性OutletParameters_outletSwitch_2的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_2", strlen("OutletParameters:outletSwitch_2")) == 0) {
        pal_OutletParameters_outletSwitch_2_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_3到云端
 * @param value 属性OutletParameters_outletSwitch_3的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_3", strlen("OutletParameters:outletSwitch_3")) == 0) {
        pal_OutletParameters_outletSwitch_3_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_4到云端
 * @param value 属性OutletParameters_outletSwitch_4的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_4", strlen("OutletParameters:outletSwitch_4")) == 0) {
        pal_OutletParameters_outletSwitch_4_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_5到云端
 * @param value 属性OutletParameters_outletSwitch_5的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_5", strlen("OutletParameters:outletSwitch_5")) == 0) {
        pal_OutletParameters_outletSwitch_5_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_6到云端
 * @param value 属性OutletParameters_outletSwitch_6的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_6", strlen("OutletParameters:outletSwitch_6")) == 0) {
        pal_OutletParameters_outletSwitch_6_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_7到云端
 * @param value 属性OutletParameters_outletSwitch_7的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_7", strlen("OutletParameters:outletSwitch_7")) == 0) {
        pal_OutletParameters_outletSwitch_7_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_8到云端
 * @param value 属性OutletParameters_outletSwitch_8的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_8", strlen("OutletParameters:outletSwitch_8")) == 0) {
        pal_OutletParameters_outletSwitch_8_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_9到云端
 * @param value 属性OutletParameters_outletSwitch_9的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_9", strlen("OutletParameters:outletSwitch_9")) == 0) {
        pal_OutletParameters_outletSwitch_9_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_10到云端
 * @param value 属性OutletParameters_outletSwitch_10的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_10", strlen("OutletParameters:outletSwitch_10")) == 0) {
        pal_OutletParameters_outletSwitch_10_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_11到云端
 * @param value 属性OutletParameters_outletSwitch_11的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_11", strlen("OutletParameters:outletSwitch_11")) == 0) {
        pal_OutletParameters_outletSwitch_11_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_12到云端
 * @param value 属性OutletParameters_outletSwitch_12的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_12", strlen("OutletParameters:outletSwitch_12")) == 0) {
        pal_OutletParameters_outletSwitch_12_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_13到云端
 * @param value 属性OutletParameters_outletSwitch_13的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_13", strlen("OutletParameters:outletSwitch_13")) == 0) {
        pal_OutletParameters_outletSwitch_13_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_14到云端
 * @param value 属性OutletParameters_outletSwitch_14的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_14", strlen("OutletParameters:outletSwitch_14")) == 0) {
        pal_OutletParameters_outletSwitch_14_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_15到云端
 * @param value 属性OutletParameters_outletSwitch_15的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_15", strlen("OutletParameters:outletSwitch_15")) == 0) {
        pal_OutletParameters_outletSwitch_15_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_16到云端
 * @param value 属性OutletParameters_outletSwitch_16的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_16", strlen("OutletParameters:outletSwitch_16")) == 0) {
        pal_OutletParameters_outletSwitch_16_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_17到云端
 * @param value 属性OutletParameters_outletSwitch_17的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_17", strlen("OutletParameters:outletSwitch_17")) == 0) {
        pal_OutletParameters_outletSwitch_17_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_18到云端
 * @param value 属性OutletParameters_outletSwitch_18的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_18", strlen("OutletParameters:outletSwitch_18")) == 0) {
        pal_OutletParameters_outletSwitch_18_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_19到云端
 * @param value 属性OutletParameters_outletSwitch_19的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_19", strlen("OutletParameters:outletSwitch_19")) == 0) {
        pal_OutletParameters_outletSwitch_19_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_20到云端
 * @param value 属性OutletParameters_outletSwitch_20的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_20", strlen("OutletParameters:outletSwitch_20")) == 0) {
        pal_OutletParameters_outletSwitch_20_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_21到云端
 * @param value 属性OutletParameters_outletSwitch_21的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_21", strlen("OutletParameters:outletSwitch_21")) == 0) {
        pal_OutletParameters_outletSwitch_21_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_22到云端
 * @param value 属性OutletParameters_outletSwitch_22的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_22", strlen("OutletParameters:outletSwitch_22")) == 0) {
        pal_OutletParameters_outletSwitch_22_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_23到云端
 * @param value 属性OutletParameters_outletSwitch_23的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_23", strlen("OutletParameters:outletSwitch_23")) == 0) {
        pal_OutletParameters_outletSwitch_23_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_24到云端
 * @param value 属性OutletParameters_outletSwitch_24的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_24", strlen("OutletParameters:outletSwitch_24")) == 0) {
        pal_OutletParameters_outletSwitch_24_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_25到云端
 * @param value 属性OutletParameters_outletSwitch_25的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_25", strlen("OutletParameters:outletSwitch_25")) == 0) {
        pal_OutletParameters_outletSwitch_25_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_26到云端
 * @param value 属性OutletParameters_outletSwitch_26的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_26", strlen("OutletParameters:outletSwitch_26")) == 0) {
        pal_OutletParameters_outletSwitch_26_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_27到云端
 * @param value 属性OutletParameters_outletSwitch_27的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_27", strlen("OutletParameters:outletSwitch_27")) == 0) {
        pal_OutletParameters_outletSwitch_27_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_28到云端
 * @param value 属性OutletParameters_outletSwitch_28的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_28", strlen("OutletParameters:outletSwitch_28")) == 0) {
        pal_OutletParameters_outletSwitch_28_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_29到云端
 * @param value 属性OutletParameters_outletSwitch_29的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_29", strlen("OutletParameters:outletSwitch_29")) == 0) {
        pal_OutletParameters_outletSwitch_29_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_30到云端
 * @param value 属性OutletParameters_outletSwitch_30的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_30", strlen("OutletParameters:outletSwitch_30")) == 0) {
        pal_OutletParameters_outletSwitch_30_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_31到云端
 * @param value 属性OutletParameters_outletSwitch_31的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_31", strlen("OutletParameters:outletSwitch_31")) == 0) {
        pal_OutletParameters_outletSwitch_31_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_32到云端
 * @param value 属性OutletParameters_outletSwitch_32的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_32", strlen("OutletParameters:outletSwitch_32")) == 0) {
        pal_OutletParameters_outletSwitch_32_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_33到云端
 * @param value 属性OutletParameters_outletSwitch_33的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_33", strlen("OutletParameters:outletSwitch_33")) == 0) {
        pal_OutletParameters_outletSwitch_33_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_34到云端
 * @param value 属性OutletParameters_outletSwitch_34的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_34", strlen("OutletParameters:outletSwitch_34")) == 0) {
        pal_OutletParameters_outletSwitch_34_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_35到云端
 * @param value 属性OutletParameters_outletSwitch_35的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_35", strlen("OutletParameters:outletSwitch_35")) == 0) {
        pal_OutletParameters_outletSwitch_35_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_36到云端
 * @param value 属性OutletParameters_outletSwitch_36的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_36", strlen("OutletParameters:outletSwitch_36")) == 0) {
        pal_OutletParameters_outletSwitch_36_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_37到云端
 * @param value 属性OutletParameters_outletSwitch_37的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_37", strlen("OutletParameters:outletSwitch_37")) == 0) {
        pal_OutletParameters_outletSwitch_37_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_38到云端
 * @param value 属性OutletParameters_outletSwitch_38的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_38", strlen("OutletParameters:outletSwitch_38")) == 0) {
        pal_OutletParameters_outletSwitch_38_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_39到云端
 * @param value 属性OutletParameters_outletSwitch_39的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_39", strlen("OutletParameters:outletSwitch_39")) == 0) {
        pal_OutletParameters_outletSwitch_39_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_40到云端
 * @param value 属性OutletParameters_outletSwitch_40的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_40", strlen("OutletParameters:outletSwitch_40")) == 0) {
        pal_OutletParameters_outletSwitch_40_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_41到云端
 * @param value 属性OutletParameters_outletSwitch_41的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_41", strlen("OutletParameters:outletSwitch_41")) == 0) {
        pal_OutletParameters_outletSwitch_41_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_42到云端
 * @param value 属性OutletParameters_outletSwitch_42的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_42", strlen("OutletParameters:outletSwitch_42")) == 0) {
        pal_OutletParameters_outletSwitch_42_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_43到云端
 * @param value 属性OutletParameters_outletSwitch_43的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_43", strlen("OutletParameters:outletSwitch_43")) == 0) {
        pal_OutletParameters_outletSwitch_43_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_44到云端
 * @param value 属性OutletParameters_outletSwitch_44的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_44", strlen("OutletParameters:outletSwitch_44")) == 0) {
        pal_OutletParameters_outletSwitch_44_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_45到云端
 * @param value 属性OutletParameters_outletSwitch_45的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_45", strlen("OutletParameters:outletSwitch_45")) == 0) {
        pal_OutletParameters_outletSwitch_45_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_46到云端
 * @param value 属性OutletParameters_outletSwitch_46的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_46", strlen("OutletParameters:outletSwitch_46")) == 0) {
        pal_OutletParameters_outletSwitch_46_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_47到云端
 * @param value 属性OutletParameters_outletSwitch_47的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_47", strlen("OutletParameters:outletSwitch_47")) == 0) {
        pal_OutletParameters_outletSwitch_47_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletSwitch_48到云端
 * @param value 属性OutletParameters_outletSwitch_48的值
 */

    if(memcmp(params + 2, "OutletParameters:outletSwitch_48", strlen("OutletParameters:outletSwitch_48")) == 0) {
        pal_OutletParameters_outletSwitch_48_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_1到云端
 * @param value 属性OutletParameters_outletEle_1的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_1", strlen("OutletParameters:outletEle_1")) == 0) {
        pal_OutletParameters_outletEle_1_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_2到云端
 * @param value 属性OutletParameters_outletEle_2的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_2", strlen("OutletParameters:outletEle_2")) == 0) {
        pal_OutletParameters_outletEle_2_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_3到云端
 * @param value 属性OutletParameters_outletEle_3的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_3", strlen("OutletParameters:outletEle_3")) == 0) {
        pal_OutletParameters_outletEle_3_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_4到云端
 * @param value 属性OutletParameters_outletEle_4的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_4", strlen("OutletParameters:outletEle_4")) == 0) {
        pal_OutletParameters_outletEle_4_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_5到云端
 * @param value 属性OutletParameters_outletEle_5的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_5", strlen("OutletParameters:outletEle_5")) == 0) {
        pal_OutletParameters_outletEle_5_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_6到云端
 * @param value 属性OutletParameters_outletEle_6的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_6", strlen("OutletParameters:outletEle_6")) == 0) {
        pal_OutletParameters_outletEle_6_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_7到云端
 * @param value 属性OutletParameters_outletEle_7的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_7", strlen("OutletParameters:outletEle_7")) == 0) {
        pal_OutletParameters_outletEle_7_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_8到云端
 * @param value 属性OutletParameters_outletEle_8的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_8", strlen("OutletParameters:outletEle_8")) == 0) {
        pal_OutletParameters_outletEle_8_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_9到云端
 * @param value 属性OutletParameters_outletEle_9的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_9", strlen("OutletParameters:outletEle_9")) == 0) {
        pal_OutletParameters_outletEle_9_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_10到云端
 * @param value 属性OutletParameters_outletEle_10的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_10", strlen("OutletParameters:outletEle_10")) == 0) {
        pal_OutletParameters_outletEle_10_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_11到云端
 * @param value 属性OutletParameters_outletEle_11的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_11", strlen("OutletParameters:outletEle_11")) == 0) {
        pal_OutletParameters_outletEle_11_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_12到云端
 * @param value 属性OutletParameters_outletEle_12的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_12", strlen("OutletParameters:outletEle_12")) == 0) {
        pal_OutletParameters_outletEle_12_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_13到云端
 * @param value 属性OutletParameters_outletEle_13的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_13", strlen("OutletParameters:outletEle_13")) == 0) {
        pal_OutletParameters_outletEle_13_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_14到云端
 * @param value 属性OutletParameters_outletEle_14的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_14", strlen("OutletParameters:outletEle_14")) == 0) {
        pal_OutletParameters_outletEle_14_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_15到云端
 * @param value 属性OutletParameters_outletEle_15的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_15", strlen("OutletParameters:outletEle_15")) == 0) {
        pal_OutletParameters_outletEle_15_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_16到云端
 * @param value 属性OutletParameters_outletEle_16的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_16", strlen("OutletParameters:outletEle_16")) == 0) {
        pal_OutletParameters_outletEle_16_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_17到云端
 * @param value 属性OutletParameters_outletEle_17的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_17", strlen("OutletParameters:outletEle_17")) == 0) {
        pal_OutletParameters_outletEle_17_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_18到云端
 * @param value 属性OutletParameters_outletEle_18的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_18", strlen("OutletParameters:outletEle_18")) == 0) {
        pal_OutletParameters_outletEle_18_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_19到云端
 * @param value 属性OutletParameters_outletEle_19的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_19", strlen("OutletParameters:outletEle_19")) == 0) {
        pal_OutletParameters_outletEle_19_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_20到云端
 * @param value 属性OutletParameters_outletEle_20的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_20", strlen("OutletParameters:outletEle_20")) == 0) {
        pal_OutletParameters_outletEle_20_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_21到云端
 * @param value 属性OutletParameters_outletEle_21的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_21", strlen("OutletParameters:outletEle_21")) == 0) {
        pal_OutletParameters_outletEle_21_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_22到云端
 * @param value 属性OutletParameters_outletEle_22的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_22", strlen("OutletParameters:outletEle_22")) == 0) {
        pal_OutletParameters_outletEle_22_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_23到云端
 * @param value 属性OutletParameters_outletEle_23的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_23", strlen("OutletParameters:outletEle_23")) == 0) {
        pal_OutletParameters_outletEle_23_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_24到云端
 * @param value 属性OutletParameters_outletEle_24的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_24", strlen("OutletParameters:outletEle_24")) == 0) {
        pal_OutletParameters_outletEle_24_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_25到云端
 * @param value 属性OutletParameters_outletEle_25的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_25", strlen("OutletParameters:outletEle_25")) == 0) {
        pal_OutletParameters_outletEle_25_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_26到云端
 * @param value 属性OutletParameters_outletEle_26的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_26", strlen("OutletParameters:outletEle_26")) == 0) {
        pal_OutletParameters_outletEle_26_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_27到云端
 * @param value 属性OutletParameters_outletEle_27的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_27", strlen("OutletParameters:outletEle_27")) == 0) {
        pal_OutletParameters_outletEle_27_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_28到云端
 * @param value 属性OutletParameters_outletEle_28的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_28", strlen("OutletParameters:outletEle_28")) == 0) {
        pal_OutletParameters_outletEle_28_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_29到云端
 * @param value 属性OutletParameters_outletEle_29的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_29", strlen("OutletParameters:outletEle_29")) == 0) {
        pal_OutletParameters_outletEle_29_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_30到云端
 * @param value 属性OutletParameters_outletEle_30的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_30", strlen("OutletParameters:outletEle_30")) == 0) {
        pal_OutletParameters_outletEle_30_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_31到云端
 * @param value 属性OutletParameters_outletEle_31的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_31", strlen("OutletParameters:outletEle_31")) == 0) {
        pal_OutletParameters_outletEle_31_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_32到云端
 * @param value 属性OutletParameters_outletEle_32的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_32", strlen("OutletParameters:outletEle_32")) == 0) {
        pal_OutletParameters_outletEle_32_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_33到云端
 * @param value 属性OutletParameters_outletEle_33的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_33", strlen("OutletParameters:outletEle_33")) == 0) {
        pal_OutletParameters_outletEle_33_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_34到云端
 * @param value 属性OutletParameters_outletEle_34的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_34", strlen("OutletParameters:outletEle_34")) == 0) {
        pal_OutletParameters_outletEle_34_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_35到云端
 * @param value 属性OutletParameters_outletEle_35的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_35", strlen("OutletParameters:outletEle_35")) == 0) {
        pal_OutletParameters_outletEle_35_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_36到云端
 * @param value 属性OutletParameters_outletEle_36的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_36", strlen("OutletParameters:outletEle_36")) == 0) {
        pal_OutletParameters_outletEle_36_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_37到云端
 * @param value 属性OutletParameters_outletEle_37的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_37", strlen("OutletParameters:outletEle_37")) == 0) {
        pal_OutletParameters_outletEle_37_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_38到云端
 * @param value 属性OutletParameters_outletEle_38的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_38", strlen("OutletParameters:outletEle_38")) == 0) {
        pal_OutletParameters_outletEle_38_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_39到云端
 * @param value 属性OutletParameters_outletEle_39的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_39", strlen("OutletParameters:outletEle_39")) == 0) {
        pal_OutletParameters_outletEle_39_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_40到云端
 * @param value 属性OutletParameters_outletEle_40的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_40", strlen("OutletParameters:outletEle_40")) == 0) {
        pal_OutletParameters_outletEle_40_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_41到云端
 * @param value 属性OutletParameters_outletEle_41的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_41", strlen("OutletParameters:outletEle_41")) == 0) {
        pal_OutletParameters_outletEle_41_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_42到云端
 * @param value 属性OutletParameters_outletEle_42的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_42", strlen("OutletParameters:outletEle_42")) == 0) {
        pal_OutletParameters_outletEle_42_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_43到云端
 * @param value 属性OutletParameters_outletEle_43的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_43", strlen("OutletParameters:outletEle_43")) == 0) {
        pal_OutletParameters_outletEle_43_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_44到云端
 * @param value 属性OutletParameters_outletEle_44的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_44", strlen("OutletParameters:outletEle_44")) == 0) {
        pal_OutletParameters_outletEle_44_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_45到云端
 * @param value 属性OutletParameters_outletEle_45的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_45", strlen("OutletParameters:outletEle_45")) == 0) {
        pal_OutletParameters_outletEle_45_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_46到云端
 * @param value 属性OutletParameters_outletEle_46的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_46", strlen("OutletParameters:outletEle_46")) == 0) {
        pal_OutletParameters_outletEle_46_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_47到云端
 * @param value 属性OutletParameters_outletEle_47的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_47", strlen("OutletParameters:outletEle_47")) == 0) {
        pal_OutletParameters_outletEle_47_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletEle_48到云端
 * @param value 属性OutletParameters_outletEle_48的值
 */

    if(memcmp(params + 2, "OutletParameters:outletEle_48", strlen("OutletParameters:outletEle_48")) == 0) {
        pal_OutletParameters_outletEle_48_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_1到云端
 * @param value 属性OutletParameters_outletPow_1的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_1", strlen("OutletParameters:outletPow_1")) == 0) {
        pal_OutletParameters_outletPow_1_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_2到云端
 * @param value 属性OutletParameters_outletPow_2的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_2", strlen("OutletParameters:outletPow_2")) == 0) {
        pal_OutletParameters_outletPow_2_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_3到云端
 * @param value 属性OutletParameters_outletPow_3的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_3", strlen("OutletParameters:outletPow_3")) == 0) {
        pal_OutletParameters_outletPow_3_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_4到云端
 * @param value 属性OutletParameters_outletPow_4的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_4", strlen("OutletParameters:outletPow_4")) == 0) {
        pal_OutletParameters_outletPow_4_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_5到云端
 * @param value 属性OutletParameters_outletPow_5的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_5", strlen("OutletParameters:outletPow_5")) == 0) {
        pal_OutletParameters_outletPow_5_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_6到云端
 * @param value 属性OutletParameters_outletPow_6的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_6", strlen("OutletParameters:outletPow_6")) == 0) {
        pal_OutletParameters_outletPow_6_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_7到云端
 * @param value 属性OutletParameters_outletPow_7的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_7", strlen("OutletParameters:outletPow_7")) == 0) {
        pal_OutletParameters_outletPow_7_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_8到云端
 * @param value 属性OutletParameters_outletPow_8的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_8", strlen("OutletParameters:outletPow_8")) == 0) {
        pal_OutletParameters_outletPow_8_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_9到云端
 * @param value 属性OutletParameters_outletPow_9的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_9", strlen("OutletParameters:outletPow_9")) == 0) {
        pal_OutletParameters_outletPow_9_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_10到云端
 * @param value 属性OutletParameters_outletPow_10的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_10", strlen("OutletParameters:outletPow_10")) == 0) {
        pal_OutletParameters_outletPow_10_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_11到云端
 * @param value 属性OutletParameters_outletPow_11的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_11", strlen("OutletParameters:outletPow_11")) == 0) {
        pal_OutletParameters_outletPow_11_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_12到云端
 * @param value 属性OutletParameters_outletPow_12的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_12", strlen("OutletParameters:outletPow_12")) == 0) {
        pal_OutletParameters_outletPow_12_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_13到云端
 * @param value 属性OutletParameters_outletPow_13的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_13", strlen("OutletParameters:outletPow_13")) == 0) {
        pal_OutletParameters_outletPow_13_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_14到云端
 * @param value 属性OutletParameters_outletPow_14的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_14", strlen("OutletParameters:outletPow_14")) == 0) {
        pal_OutletParameters_outletPow_14_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_15到云端
 * @param value 属性OutletParameters_outletPow_15的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_15", strlen("OutletParameters:outletPow_15")) == 0) {
        pal_OutletParameters_outletPow_15_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_16到云端
 * @param value 属性OutletParameters_outletPow_16的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_16", strlen("OutletParameters:outletPow_16")) == 0) {
        pal_OutletParameters_outletPow_16_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_17到云端
 * @param value 属性OutletParameters_outletPow_17的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_17", strlen("OutletParameters:outletPow_17")) == 0) {
        pal_OutletParameters_outletPow_17_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_18到云端
 * @param value 属性OutletParameters_outletPow_18的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_18", strlen("OutletParameters:outletPow_18")) == 0) {
        pal_OutletParameters_outletPow_18_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_19到云端
 * @param value 属性OutletParameters_outletPow_19的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_19", strlen("OutletParameters:outletPow_19")) == 0) {
        pal_OutletParameters_outletPow_19_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_20到云端
 * @param value 属性OutletParameters_outletPow_20的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_20", strlen("OutletParameters:outletPow_20")) == 0) {
        pal_OutletParameters_outletPow_20_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_21到云端
 * @param value 属性OutletParameters_outletPow_21的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_21", strlen("OutletParameters:outletPow_21")) == 0) {
        pal_OutletParameters_outletPow_21_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_22到云端
 * @param value 属性OutletParameters_outletPow_22的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_22", strlen("OutletParameters:outletPow_22")) == 0) {
        pal_OutletParameters_outletPow_22_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_23到云端
 * @param value 属性OutletParameters_outletPow_23的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_23", strlen("OutletParameters:outletPow_23")) == 0) {
        pal_OutletParameters_outletPow_23_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_24到云端
 * @param value 属性OutletParameters_outletPow_24的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_24", strlen("OutletParameters:outletPow_24")) == 0) {
        pal_OutletParameters_outletPow_24_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_25到云端
 * @param value 属性OutletParameters_outletPow_25的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_25", strlen("OutletParameters:outletPow_25")) == 0) {
        pal_OutletParameters_outletPow_25_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_26到云端
 * @param value 属性OutletParameters_outletPow_26的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_26", strlen("OutletParameters:outletPow_26")) == 0) {
        pal_OutletParameters_outletPow_26_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_27到云端
 * @param value 属性OutletParameters_outletPow_27的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_27", strlen("OutletParameters:outletPow_27")) == 0) {
        pal_OutletParameters_outletPow_27_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_28到云端
 * @param value 属性OutletParameters_outletPow_28的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_28", strlen("OutletParameters:outletPow_28")) == 0) {
        pal_OutletParameters_outletPow_28_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_29到云端
 * @param value 属性OutletParameters_outletPow_29的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_29", strlen("OutletParameters:outletPow_29")) == 0) {
        pal_OutletParameters_outletPow_29_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_30到云端
 * @param value 属性OutletParameters_outletPow_30的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_30", strlen("OutletParameters:outletPow_30")) == 0) {
        pal_OutletParameters_outletPow_30_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_31到云端
 * @param value 属性OutletParameters_outletPow_31的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_31", strlen("OutletParameters:outletPow_31")) == 0) {
        pal_OutletParameters_outletPow_31_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_32到云端
 * @param value 属性OutletParameters_outletPow_32的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_32", strlen("OutletParameters:outletPow_32")) == 0) {
        pal_OutletParameters_outletPow_32_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_33到云端
 * @param value 属性OutletParameters_outletPow_33的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_33", strlen("OutletParameters:outletPow_33")) == 0) {
        pal_OutletParameters_outletPow_33_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_34到云端
 * @param value 属性OutletParameters_outletPow_34的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_34", strlen("OutletParameters:outletPow_34")) == 0) {
        pal_OutletParameters_outletPow_34_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_35到云端
 * @param value 属性OutletParameters_outletPow_35的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_35", strlen("OutletParameters:outletPow_35")) == 0) {
        pal_OutletParameters_outletPow_35_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_36到云端
 * @param value 属性OutletParameters_outletPow_36的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_36", strlen("OutletParameters:outletPow_36")) == 0) {
        pal_OutletParameters_outletPow_36_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_37到云端
 * @param value 属性OutletParameters_outletPow_37的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_37", strlen("OutletParameters:outletPow_37")) == 0) {
        pal_OutletParameters_outletPow_37_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_38到云端
 * @param value 属性OutletParameters_outletPow_38的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_38", strlen("OutletParameters:outletPow_38")) == 0) {
        pal_OutletParameters_outletPow_38_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_39到云端
 * @param value 属性OutletParameters_outletPow_39的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_39", strlen("OutletParameters:outletPow_39")) == 0) {
        pal_OutletParameters_outletPow_39_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_40到云端
 * @param value 属性OutletParameters_outletPow_40的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_40", strlen("OutletParameters:outletPow_40")) == 0) {
        pal_OutletParameters_outletPow_40_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_41到云端
 * @param value 属性OutletParameters_outletPow_41的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_41", strlen("OutletParameters:outletPow_41")) == 0) {
        pal_OutletParameters_outletPow_41_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_42到云端
 * @param value 属性OutletParameters_outletPow_42的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_42", strlen("OutletParameters:outletPow_42")) == 0) {
        pal_OutletParameters_outletPow_42_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_43到云端
 * @param value 属性OutletParameters_outletPow_43的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_43", strlen("OutletParameters:outletPow_43")) == 0) {
        pal_OutletParameters_outletPow_43_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_44到云端
 * @param value 属性OutletParameters_outletPow_44的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_44", strlen("OutletParameters:outletPow_44")) == 0) {
        pal_OutletParameters_outletPow_44_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_45到云端
 * @param value 属性OutletParameters_outletPow_45的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_45", strlen("OutletParameters:outletPow_45")) == 0) {
        pal_OutletParameters_outletPow_45_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_46到云端
 * @param value 属性OutletParameters_outletPow_46的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_46", strlen("OutletParameters:outletPow_46")) == 0) {
        pal_OutletParameters_outletPow_46_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_47到云端
 * @param value 属性OutletParameters_outletPow_47的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_47", strlen("OutletParameters:outletPow_47")) == 0) {
        pal_OutletParameters_outletPow_47_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性OutletParameters_outletPow_48到云端
 * @param value 属性OutletParameters_outletPow_48的值
 */

    if(memcmp(params + 2, "OutletParameters:outletPow_48", strlen("OutletParameters:outletPow_48")) == 0) {
        pal_OutletParameters_outletPow_48_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性DevCfg_lineNum到云端
 * @param value 属性DevCfg_lineNum的值
 */

    if(memcmp(params + 2, "DevCfg:lineNum", strlen("DevCfg:lineNum")) == 0) {
        pal_DevCfg_lineNum_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性DevCfg_loopNum到云端
 * @param value 属性DevCfg_loopNum的值
 */

    if(memcmp(params + 2, "DevCfg:loopNum", strlen("DevCfg:loopNum")) == 0) {
        pal_DevCfg_loopNum_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性DevCfg_outletNum到云端
 * @param value 属性DevCfg_outletNum的值
 */

    if(memcmp(params + 2, "DevCfg:outletNum", strlen("DevCfg:outletNum")) == 0) {
        pal_DevCfg_outletNum_property_set(params, len);
        return;
    }
/**
 * @brief 上报属性runningState到云端
 * @param value 属性runningState的值
 */

    if(memcmp(params + 2, "runningState", strlen("runningState")) == 0) {
        pal_runningState_property_set(params, len);
        return;
    }

}
