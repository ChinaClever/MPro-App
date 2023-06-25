#include <stdio.h>
#include <string.h>
#include "aiot_dm_api.h"

/**
 * @brief 上报属性runningState到云端
 * @param value 属性runningState的值
 * @param value 属性runningState的值，数据类型int32_t
 */

int32_t pal_post_property_runningState (void *dm_handle, int32_t value);
/**
 * @brief 上报属性lineVol_1到云端
 * @param value 属性lineVol_1的值
 * @param value 属性lineVol_1的值，数据类型double
 */

int32_t pal_post_property_lineVol_1 (void *dm_handle, double value);
/**
 * @brief 上报属性lineVol_2到云端
 * @param value 属性lineVol_2的值
 * @param value 属性lineVol_2的值，数据类型double
 */

int32_t pal_post_property_lineVol_2 (void *dm_handle, double value);
/**
 * @brief 上报属性lineVol_3到云端
 * @param value 属性lineVol_3的值
 * @param value 属性lineVol_3的值，数据类型float
 */

int32_t pal_post_property_lineVol_3 (void *dm_handle, float value);
/**
 * @brief 上报属性linePow_1到云端
 * @param value 属性linePow_1的值
 * @param value 属性linePow_1的值，数据类型double
 */

int32_t pal_post_property_linePow_1 (void *dm_handle, double value);
/**
 * @brief 上报属性linePow_2到云端
 * @param value 属性linePow_2的值
 * @param value 属性linePow_2的值，数据类型double
 */

int32_t pal_post_property_linePow_2 (void *dm_handle, double value);
/**
 * @brief 上报属性linePow_3到云端
 * @param value 属性linePow_3的值
 * @param value 属性linePow_3的值，数据类型double
 */

int32_t pal_post_property_linePow_3 (void *dm_handle, double value);
/**
 * @brief 上报属性totalActivePower到云端
 * @param value 属性totalActivePower的值
 * @param value 属性totalActivePower的值，数据类型float
 */

int32_t pal_post_property_totalActivePower (void *dm_handle, float value);
/**
 * @brief 上报属性totalApparentPower到云端
 * @param value 属性totalApparentPower的值
 * @param value 属性totalApparentPower的值，数据类型double
 */

int32_t pal_post_property_totalApparentPower (void *dm_handle, double value);
/**
 * @brief 上报属性totalPowerFactor到云端
 * @param value 属性totalPowerFactor的值
 * @param value 属性totalPowerFactor的值，数据类型int32_t
 */

int32_t pal_post_property_totalPowerFactor (void *dm_handle, int32_t value);
/**
 * @brief 上报属性totalActiveEnergy到云端
 * @param value 属性totalActiveEnergy的值
 * @param value 属性totalActiveEnergy的值，数据类型double
 */

int32_t pal_post_property_totalActiveEnergy (void *dm_handle, double value);
/**
 * @brief 上报属性Breaker_1到云端
 * @param value 属性Breaker_1的值
 * @param value 属性Breaker_1的值，数据类型uint8_t
 */

int32_t pal_post_property_Breaker_1 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性Breaker_2到云端
 * @param value 属性Breaker_2的值
 * @param value 属性Breaker_2的值，数据类型uint8_t
 */

int32_t pal_post_property_Breaker_2 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性Breaker_3到云端
 * @param value 属性Breaker_3的值
 * @param value 属性Breaker_3的值，数据类型uint8_t
 */

int32_t pal_post_property_Breaker_3 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性Breaker_4到云端
 * @param value 属性Breaker_4的值
 * @param value 属性Breaker_4的值，数据类型uint8_t
 */

int32_t pal_post_property_Breaker_4 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性Breaker_5到云端
 * @param value 属性Breaker_5的值
 * @param value 属性Breaker_5的值，数据类型uint8_t
 */

int32_t pal_post_property_Breaker_5 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性Breaker_6到云端
 * @param value 属性Breaker_6的值
 * @param value 属性Breaker_6的值，数据类型uint8_t
 */

int32_t pal_post_property_Breaker_6 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性loopActivePower_1到云端
 * @param value 属性loopActivePower_1的值
 * @param value 属性loopActivePower_1的值，数据类型float
 */

int32_t pal_post_property_loopActivePower_1 (void *dm_handle, float value);
/**
 * @brief 上报属性loopActivePower_2到云端
 * @param value 属性loopActivePower_2的值
 * @param value 属性loopActivePower_2的值，数据类型float
 */

int32_t pal_post_property_loopActivePower_2 (void *dm_handle, float value);
/**
 * @brief 上报属性loopActivePower_3到云端
 * @param value 属性loopActivePower_3的值
 * @param value 属性loopActivePower_3的值，数据类型float
 */

int32_t pal_post_property_loopActivePower_3 (void *dm_handle, float value);
/**
 * @brief 上报属性loopActivePower_4到云端
 * @param value 属性loopActivePower_4的值
 * @param value 属性loopActivePower_4的值，数据类型float
 */

int32_t pal_post_property_loopActivePower_4 (void *dm_handle, float value);
/**
 * @brief 上报属性loopActivePower_5到云端
 * @param value 属性loopActivePower_5的值
 * @param value 属性loopActivePower_5的值，数据类型float
 */

int32_t pal_post_property_loopActivePower_5 (void *dm_handle, float value);
/**
 * @brief 上报属性loopActivePower_6到云端
 * @param value 属性loopActivePower_6的值
 * @param value 属性loopActivePower_6的值，数据类型float
 */

int32_t pal_post_property_loopActivePower_6 (void *dm_handle, float value);
/**
 * @brief 上报属性outletSwitch_1到云端
 * @param value 属性outletSwitch_1的值
 * @param value 属性outletSwitch_1的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_1 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_2到云端
 * @param value 属性outletSwitch_2的值
 * @param value 属性outletSwitch_2的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_2 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_3到云端
 * @param value 属性outletSwitch_3的值
 * @param value 属性outletSwitch_3的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_3 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_4到云端
 * @param value 属性outletSwitch_4的值
 * @param value 属性outletSwitch_4的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_4 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_5到云端
 * @param value 属性outletSwitch_5的值
 * @param value 属性outletSwitch_5的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_5 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_6到云端
 * @param value 属性outletSwitch_6的值
 * @param value 属性outletSwitch_6的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_6 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_7到云端
 * @param value 属性outletSwitch_7的值
 * @param value 属性outletSwitch_7的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_7 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_8到云端
 * @param value 属性outletSwitch_8的值
 * @param value 属性outletSwitch_8的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_8 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_9到云端
 * @param value 属性outletSwitch_9的值
 * @param value 属性outletSwitch_9的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_9 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_10到云端
 * @param value 属性outletSwitch_10的值
 * @param value 属性outletSwitch_10的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_10 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_11到云端
 * @param value 属性outletSwitch_11的值
 * @param value 属性outletSwitch_11的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_11 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_12到云端
 * @param value 属性outletSwitch_12的值
 * @param value 属性outletSwitch_12的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_12 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_13到云端
 * @param value 属性outletSwitch_13的值
 * @param value 属性outletSwitch_13的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_13 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_14到云端
 * @param value 属性outletSwitch_14的值
 * @param value 属性outletSwitch_14的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_14 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_15到云端
 * @param value 属性outletSwitch_15的值
 * @param value 属性outletSwitch_15的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_15 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_16到云端
 * @param value 属性outletSwitch_16的值
 * @param value 属性outletSwitch_16的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_16 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_17到云端
 * @param value 属性outletSwitch_17的值
 * @param value 属性outletSwitch_17的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_17 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_18到云端
 * @param value 属性outletSwitch_18的值
 * @param value 属性outletSwitch_18的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_18 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_19到云端
 * @param value 属性outletSwitch_19的值
 * @param value 属性outletSwitch_19的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_19 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_20到云端
 * @param value 属性outletSwitch_20的值
 * @param value 属性outletSwitch_20的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_20 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_21到云端
 * @param value 属性outletSwitch_21的值
 * @param value 属性outletSwitch_21的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_21 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_22到云端
 * @param value 属性outletSwitch_22的值
 * @param value 属性outletSwitch_22的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_22 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_23到云端
 * @param value 属性outletSwitch_23的值
 * @param value 属性outletSwitch_23的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_23 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_24到云端
 * @param value 属性outletSwitch_24的值
 * @param value 属性outletSwitch_24的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_24 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_25到云端
 * @param value 属性outletSwitch_25的值
 * @param value 属性outletSwitch_25的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_25 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_26到云端
 * @param value 属性outletSwitch_26的值
 * @param value 属性outletSwitch_26的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_26 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_27到云端
 * @param value 属性outletSwitch_27的值
 * @param value 属性outletSwitch_27的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_27 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_28到云端
 * @param value 属性outletSwitch_28的值
 * @param value 属性outletSwitch_28的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_28 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_29到云端
 * @param value 属性outletSwitch_29的值
 * @param value 属性outletSwitch_29的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_29 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_30到云端
 * @param value 属性outletSwitch_30的值
 * @param value 属性outletSwitch_30的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_30 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_31到云端
 * @param value 属性outletSwitch_31的值
 * @param value 属性outletSwitch_31的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_31 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_32到云端
 * @param value 属性outletSwitch_32的值
 * @param value 属性outletSwitch_32的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_32 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_33到云端
 * @param value 属性outletSwitch_33的值
 * @param value 属性outletSwitch_33的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_33 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_34到云端
 * @param value 属性outletSwitch_34的值
 * @param value 属性outletSwitch_34的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_34 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_35到云端
 * @param value 属性outletSwitch_35的值
 * @param value 属性outletSwitch_35的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_35 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_36到云端
 * @param value 属性outletSwitch_36的值
 * @param value 属性outletSwitch_36的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_36 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_37到云端
 * @param value 属性outletSwitch_37的值
 * @param value 属性outletSwitch_37的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_37 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_38到云端
 * @param value 属性outletSwitch_38的值
 * @param value 属性outletSwitch_38的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_38 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_39到云端
 * @param value 属性outletSwitch_39的值
 * @param value 属性outletSwitch_39的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_39 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_40到云端
 * @param value 属性outletSwitch_40的值
 * @param value 属性outletSwitch_40的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_40 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_41到云端
 * @param value 属性outletSwitch_41的值
 * @param value 属性outletSwitch_41的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_41 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_42到云端
 * @param value 属性outletSwitch_42的值
 * @param value 属性outletSwitch_42的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_42 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_43到云端
 * @param value 属性outletSwitch_43的值
 * @param value 属性outletSwitch_43的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_43 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_44到云端
 * @param value 属性outletSwitch_44的值
 * @param value 属性outletSwitch_44的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_44 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_45到云端
 * @param value 属性outletSwitch_45的值
 * @param value 属性outletSwitch_45的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_45 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_46到云端
 * @param value 属性outletSwitch_46的值
 * @param value 属性outletSwitch_46的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_46 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_47到云端
 * @param value 属性outletSwitch_47的值
 * @param value 属性outletSwitch_47的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_47 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletSwitch_48到云端
 * @param value 属性outletSwitch_48的值
 * @param value 属性outletSwitch_48的值，数据类型uint8_t
 */

int32_t pal_post_property_outletSwitch_48 (void *dm_handle, uint8_t value);
/**
 * @brief 上报属性outletEle_1到云端
 * @param value 属性outletEle_1的值
 * @param value 属性outletEle_1的值，数据类型double
 */

int32_t pal_post_property_outletEle_1 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_2到云端
 * @param value 属性outletEle_2的值
 * @param value 属性outletEle_2的值，数据类型double
 */

int32_t pal_post_property_outletEle_2 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_3到云端
 * @param value 属性outletEle_3的值
 * @param value 属性outletEle_3的值，数据类型double
 */

int32_t pal_post_property_outletEle_3 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_4到云端
 * @param value 属性outletEle_4的值
 * @param value 属性outletEle_4的值，数据类型double
 */

int32_t pal_post_property_outletEle_4 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_5到云端
 * @param value 属性outletEle_5的值
 * @param value 属性outletEle_5的值，数据类型double
 */

int32_t pal_post_property_outletEle_5 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_6到云端
 * @param value 属性outletEle_6的值
 * @param value 属性outletEle_6的值，数据类型double
 */

int32_t pal_post_property_outletEle_6 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_7到云端
 * @param value 属性outletEle_7的值
 * @param value 属性outletEle_7的值，数据类型double
 */

int32_t pal_post_property_outletEle_7 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_8到云端
 * @param value 属性outletEle_8的值
 * @param value 属性outletEle_8的值，数据类型double
 */

int32_t pal_post_property_outletEle_8 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_9到云端
 * @param value 属性outletEle_9的值
 * @param value 属性outletEle_9的值，数据类型double
 */

int32_t pal_post_property_outletEle_9 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_10到云端
 * @param value 属性outletEle_10的值
 * @param value 属性outletEle_10的值，数据类型double
 */

int32_t pal_post_property_outletEle_10 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_11到云端
 * @param value 属性outletEle_11的值
 * @param value 属性outletEle_11的值，数据类型double
 */

int32_t pal_post_property_outletEle_11 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_12到云端
 * @param value 属性outletEle_12的值
 * @param value 属性outletEle_12的值，数据类型double
 */

int32_t pal_post_property_outletEle_12 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_13到云端
 * @param value 属性outletEle_13的值
 * @param value 属性outletEle_13的值，数据类型double
 */

int32_t pal_post_property_outletEle_13 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_14到云端
 * @param value 属性outletEle_14的值
 * @param value 属性outletEle_14的值，数据类型double
 */

int32_t pal_post_property_outletEle_14 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_15到云端
 * @param value 属性outletEle_15的值
 * @param value 属性outletEle_15的值，数据类型double
 */

int32_t pal_post_property_outletEle_15 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_16到云端
 * @param value 属性outletEle_16的值
 * @param value 属性outletEle_16的值，数据类型double
 */

int32_t pal_post_property_outletEle_16 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_17到云端
 * @param value 属性outletEle_17的值
 * @param value 属性outletEle_17的值，数据类型double
 */

int32_t pal_post_property_outletEle_17 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_18到云端
 * @param value 属性outletEle_18的值
 * @param value 属性outletEle_18的值，数据类型double
 */

int32_t pal_post_property_outletEle_18 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_19到云端
 * @param value 属性outletEle_19的值
 * @param value 属性outletEle_19的值，数据类型double
 */

int32_t pal_post_property_outletEle_19 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_20到云端
 * @param value 属性outletEle_20的值
 * @param value 属性outletEle_20的值，数据类型double
 */

int32_t pal_post_property_outletEle_20 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_21到云端
 * @param value 属性outletEle_21的值
 * @param value 属性outletEle_21的值，数据类型double
 */

int32_t pal_post_property_outletEle_21 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_22到云端
 * @param value 属性outletEle_22的值
 * @param value 属性outletEle_22的值，数据类型double
 */

int32_t pal_post_property_outletEle_22 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_23到云端
 * @param value 属性outletEle_23的值
 * @param value 属性outletEle_23的值，数据类型double
 */

int32_t pal_post_property_outletEle_23 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_24到云端
 * @param value 属性outletEle_24的值
 * @param value 属性outletEle_24的值，数据类型double
 */

int32_t pal_post_property_outletEle_24 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_25到云端
 * @param value 属性outletEle_25的值
 * @param value 属性outletEle_25的值，数据类型double
 */

int32_t pal_post_property_outletEle_25 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_26到云端
 * @param value 属性outletEle_26的值
 * @param value 属性outletEle_26的值，数据类型double
 */

int32_t pal_post_property_outletEle_26 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_27到云端
 * @param value 属性outletEle_27的值
 * @param value 属性outletEle_27的值，数据类型double
 */

int32_t pal_post_property_outletEle_27 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_28到云端
 * @param value 属性outletEle_28的值
 * @param value 属性outletEle_28的值，数据类型double
 */

int32_t pal_post_property_outletEle_28 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_29到云端
 * @param value 属性outletEle_29的值
 * @param value 属性outletEle_29的值，数据类型double
 */

int32_t pal_post_property_outletEle_29 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_30到云端
 * @param value 属性outletEle_30的值
 * @param value 属性outletEle_30的值，数据类型double
 */

int32_t pal_post_property_outletEle_30 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_31到云端
 * @param value 属性outletEle_31的值
 * @param value 属性outletEle_31的值，数据类型double
 */

int32_t pal_post_property_outletEle_31 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_32到云端
 * @param value 属性outletEle_32的值
 * @param value 属性outletEle_32的值，数据类型double
 */

int32_t pal_post_property_outletEle_32 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_33到云端
 * @param value 属性outletEle_33的值
 * @param value 属性outletEle_33的值，数据类型double
 */

int32_t pal_post_property_outletEle_33 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_34到云端
 * @param value 属性outletEle_34的值
 * @param value 属性outletEle_34的值，数据类型double
 */

int32_t pal_post_property_outletEle_34 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_35到云端
 * @param value 属性outletEle_35的值
 * @param value 属性outletEle_35的值，数据类型double
 */

int32_t pal_post_property_outletEle_35 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_36到云端
 * @param value 属性outletEle_36的值
 * @param value 属性outletEle_36的值，数据类型double
 */

int32_t pal_post_property_outletEle_36 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_37到云端
 * @param value 属性outletEle_37的值
 * @param value 属性outletEle_37的值，数据类型double
 */

int32_t pal_post_property_outletEle_37 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_38到云端
 * @param value 属性outletEle_38的值
 * @param value 属性outletEle_38的值，数据类型double
 */

int32_t pal_post_property_outletEle_38 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_39到云端
 * @param value 属性outletEle_39的值
 * @param value 属性outletEle_39的值，数据类型double
 */

int32_t pal_post_property_outletEle_39 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_40到云端
 * @param value 属性outletEle_40的值
 * @param value 属性outletEle_40的值，数据类型double
 */

int32_t pal_post_property_outletEle_40 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_41到云端
 * @param value 属性outletEle_41的值
 * @param value 属性outletEle_41的值，数据类型double
 */

int32_t pal_post_property_outletEle_41 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_42到云端
 * @param value 属性outletEle_42的值
 * @param value 属性outletEle_42的值，数据类型double
 */

int32_t pal_post_property_outletEle_42 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_43到云端
 * @param value 属性outletEle_43的值
 * @param value 属性outletEle_43的值，数据类型double
 */

int32_t pal_post_property_outletEle_43 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_44到云端
 * @param value 属性outletEle_44的值
 * @param value 属性outletEle_44的值，数据类型double
 */

int32_t pal_post_property_outletEle_44 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_45到云端
 * @param value 属性outletEle_45的值
 * @param value 属性outletEle_45的值，数据类型double
 */

int32_t pal_post_property_outletEle_45 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_46到云端
 * @param value 属性outletEle_46的值
 * @param value 属性outletEle_46的值，数据类型double
 */

int32_t pal_post_property_outletEle_46 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_47到云端
 * @param value 属性outletEle_47的值
 * @param value 属性outletEle_47的值，数据类型double
 */

int32_t pal_post_property_outletEle_47 (void *dm_handle, double value);
/**
 * @brief 上报属性outletEle_48到云端
 * @param value 属性outletEle_48的值
 * @param value 属性outletEle_48的值，数据类型double
 */

int32_t pal_post_property_outletEle_48 (void *dm_handle, double value);
/**
 * @brief 上报属性outletPow_1到云端
 * @param value 属性outletPow_1的值
 * @param value 属性outletPow_1的值，数据类型float
 */

int32_t pal_post_property_outletPow_1 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_2到云端
 * @param value 属性outletPow_2的值
 * @param value 属性outletPow_2的值，数据类型float
 */

int32_t pal_post_property_outletPow_2 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_3到云端
 * @param value 属性outletPow_3的值
 * @param value 属性outletPow_3的值，数据类型float
 */

int32_t pal_post_property_outletPow_3 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_4到云端
 * @param value 属性outletPow_4的值
 * @param value 属性outletPow_4的值，数据类型float
 */

int32_t pal_post_property_outletPow_4 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_5到云端
 * @param value 属性outletPow_5的值
 * @param value 属性outletPow_5的值，数据类型float
 */

int32_t pal_post_property_outletPow_5 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_6到云端
 * @param value 属性outletPow_6的值
 * @param value 属性outletPow_6的值，数据类型float
 */

int32_t pal_post_property_outletPow_6 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_7到云端
 * @param value 属性outletPow_7的值
 * @param value 属性outletPow_7的值，数据类型float
 */

int32_t pal_post_property_outletPow_7 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_8到云端
 * @param value 属性outletPow_8的值
 * @param value 属性outletPow_8的值，数据类型float
 */

int32_t pal_post_property_outletPow_8 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_9到云端
 * @param value 属性outletPow_9的值
 * @param value 属性outletPow_9的值，数据类型float
 */

int32_t pal_post_property_outletPow_9 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_10到云端
 * @param value 属性outletPow_10的值
 * @param value 属性outletPow_10的值，数据类型float
 */

int32_t pal_post_property_outletPow_10 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_11到云端
 * @param value 属性outletPow_11的值
 * @param value 属性outletPow_11的值，数据类型float
 */

int32_t pal_post_property_outletPow_11 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_12到云端
 * @param value 属性outletPow_12的值
 * @param value 属性outletPow_12的值，数据类型float
 */

int32_t pal_post_property_outletPow_12 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_13到云端
 * @param value 属性outletPow_13的值
 * @param value 属性outletPow_13的值，数据类型float
 */

int32_t pal_post_property_outletPow_13 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_14到云端
 * @param value 属性outletPow_14的值
 * @param value 属性outletPow_14的值，数据类型float
 */

int32_t pal_post_property_outletPow_14 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_15到云端
 * @param value 属性outletPow_15的值
 * @param value 属性outletPow_15的值，数据类型float
 */

int32_t pal_post_property_outletPow_15 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_16到云端
 * @param value 属性outletPow_16的值
 * @param value 属性outletPow_16的值，数据类型float
 */

int32_t pal_post_property_outletPow_16 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_17到云端
 * @param value 属性outletPow_17的值
 * @param value 属性outletPow_17的值，数据类型float
 */

int32_t pal_post_property_outletPow_17 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_18到云端
 * @param value 属性outletPow_18的值
 * @param value 属性outletPow_18的值，数据类型float
 */

int32_t pal_post_property_outletPow_18 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_19到云端
 * @param value 属性outletPow_19的值
 * @param value 属性outletPow_19的值，数据类型float
 */

int32_t pal_post_property_outletPow_19 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_20到云端
 * @param value 属性outletPow_20的值
 * @param value 属性outletPow_20的值，数据类型float
 */

int32_t pal_post_property_outletPow_20 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_21到云端
 * @param value 属性outletPow_21的值
 * @param value 属性outletPow_21的值，数据类型float
 */

int32_t pal_post_property_outletPow_21 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_22到云端
 * @param value 属性outletPow_22的值
 * @param value 属性outletPow_22的值，数据类型float
 */

int32_t pal_post_property_outletPow_22 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_23到云端
 * @param value 属性outletPow_23的值
 * @param value 属性outletPow_23的值，数据类型float
 */

int32_t pal_post_property_outletPow_23 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_24到云端
 * @param value 属性outletPow_24的值
 * @param value 属性outletPow_24的值，数据类型float
 */

int32_t pal_post_property_outletPow_24 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_25到云端
 * @param value 属性outletPow_25的值
 * @param value 属性outletPow_25的值，数据类型float
 */

int32_t pal_post_property_outletPow_25 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_26到云端
 * @param value 属性outletPow_26的值
 * @param value 属性outletPow_26的值，数据类型float
 */

int32_t pal_post_property_outletPow_26 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_27到云端
 * @param value 属性outletPow_27的值
 * @param value 属性outletPow_27的值，数据类型float
 */

int32_t pal_post_property_outletPow_27 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_28到云端
 * @param value 属性outletPow_28的值
 * @param value 属性outletPow_28的值，数据类型float
 */

int32_t pal_post_property_outletPow_28 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_29到云端
 * @param value 属性outletPow_29的值
 * @param value 属性outletPow_29的值，数据类型float
 */

int32_t pal_post_property_outletPow_29 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_30到云端
 * @param value 属性outletPow_30的值
 * @param value 属性outletPow_30的值，数据类型float
 */

int32_t pal_post_property_outletPow_30 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_31到云端
 * @param value 属性outletPow_31的值
 * @param value 属性outletPow_31的值，数据类型float
 */

int32_t pal_post_property_outletPow_31 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_32到云端
 * @param value 属性outletPow_32的值
 * @param value 属性outletPow_32的值，数据类型float
 */

int32_t pal_post_property_outletPow_32 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_33到云端
 * @param value 属性outletPow_33的值
 * @param value 属性outletPow_33的值，数据类型float
 */

int32_t pal_post_property_outletPow_33 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_34到云端
 * @param value 属性outletPow_34的值
 * @param value 属性outletPow_34的值，数据类型float
 */

int32_t pal_post_property_outletPow_34 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_35到云端
 * @param value 属性outletPow_35的值
 * @param value 属性outletPow_35的值，数据类型float
 */

int32_t pal_post_property_outletPow_35 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_36到云端
 * @param value 属性outletPow_36的值
 * @param value 属性outletPow_36的值，数据类型float
 */

int32_t pal_post_property_outletPow_36 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_37到云端
 * @param value 属性outletPow_37的值
 * @param value 属性outletPow_37的值，数据类型float
 */

int32_t pal_post_property_outletPow_37 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_38到云端
 * @param value 属性outletPow_38的值
 * @param value 属性outletPow_38的值，数据类型float
 */

int32_t pal_post_property_outletPow_38 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_39到云端
 * @param value 属性outletPow_39的值
 * @param value 属性outletPow_39的值，数据类型float
 */

int32_t pal_post_property_outletPow_39 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_40到云端
 * @param value 属性outletPow_40的值
 * @param value 属性outletPow_40的值，数据类型float
 */

int32_t pal_post_property_outletPow_40 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_41到云端
 * @param value 属性outletPow_41的值
 * @param value 属性outletPow_41的值，数据类型float
 */

int32_t pal_post_property_outletPow_41 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_42到云端
 * @param value 属性outletPow_42的值
 * @param value 属性outletPow_42的值，数据类型float
 */

int32_t pal_post_property_outletPow_42 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_43到云端
 * @param value 属性outletPow_43的值
 * @param value 属性outletPow_43的值，数据类型float
 */

int32_t pal_post_property_outletPow_43 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_44到云端
 * @param value 属性outletPow_44的值
 * @param value 属性outletPow_44的值，数据类型float
 */

int32_t pal_post_property_outletPow_44 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_45到云端
 * @param value 属性outletPow_45的值
 * @param value 属性outletPow_45的值，数据类型float
 */

int32_t pal_post_property_outletPow_45 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_46到云端
 * @param value 属性outletPow_46的值
 * @param value 属性outletPow_46的值，数据类型float
 */

int32_t pal_post_property_outletPow_46 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_47到云端
 * @param value 属性outletPow_47的值
 * @param value 属性outletPow_47的值，数据类型float
 */

int32_t pal_post_property_outletPow_47 (void *dm_handle, float value);
/**
 * @brief 上报属性outletPow_48到云端
 * @param value 属性outletPow_48的值
 * @param value 属性outletPow_48的值，数据类型float
 */

int32_t pal_post_property_outletPow_48 (void *dm_handle, float value);
/**
 * @brief 上报属性lineNum到云端
 * @param value 属性lineNum的值
 * @param value 属性lineNum的值，数据类型int32_t
 */

int32_t pal_post_property_lineNum (void *dm_handle, int32_t value);
/**
 * @brief 上报属性loopNum到云端
 * @param value 属性loopNum的值
 * @param value 属性loopNum的值，数据类型int32_t
 */

int32_t pal_post_property_loopNum (void *dm_handle, int32_t value);
/**
 * @brief 上报属性outletNum到云端
 * @param value 属性outletNum的值
 * @param value 属性outletNum的值，数据类型int32_t
 */

int32_t pal_post_property_outletNum (void *dm_handle, int32_t value);
