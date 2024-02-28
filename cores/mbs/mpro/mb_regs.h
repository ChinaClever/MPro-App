#ifndef MB_REGS_H
#define MB_REGS_H
#include "modbus_slavertu.h"
#include "set_core.h"
#include "op_core.h"

enum eMbRegs {
    MbReg_Factory   =   0,     // 生产厂家
    MbReg_Model     =   4,    // 型号 例：HVPDU63-2-32(01)
    MbReg_Hw       =   20,    // A-Z
    MbReg_Sw       =   21,    // 100-999
    MbReg_Version  =   22,    // 0x100-0x999 0x100-0x999
    MbReg_CompileTime  = 24,  // 软件编译时间 YYYYMMDD（字符串）

    MbReg_LineData = 101, // 相电压、相电流、相有功功率
    MbReg_LineAlarm = 1011,
    MbReg_LineThreshol = 3101,

    MbReg_LoopData = 131,
    MbReg_LoopBreaker = 1001,
    MbReg_LoopAlarm = 1021,
    MbReg_LoopThreshol = 3201,

    MbReg_OutputValue = 201, // 输出位参数
    MbReg_OutputData = 501, // 输出位参数
    MbReg_OutputAlarm = 1201,
    MbReg_OutputRelay = 3901,
    MbReg_OutputEle = 3951,
    MbReg_OutputThreshol = 3301,

    MbReg_GroupData = 1601,
    MbReg_GroupThreshol = 4101,
    MbReg_GroupRelay = 4176,

    MbReg_EnvData = 1951,
    MbReg_EnvAlarm = 1961,
    MbReg_EnvSensor = 1971,
    MbReg_EnvThreshol = 4001,

    MbReg_Setting = 3000,    // 配置参数
    MbReg_SetAddr = 3001, // 通讯地址
    MbReg_SetBuzzer = 3002, // 蜂鸣器开关
    MbReg_SetTime = 3004,
    MbReg_SetEle = 3003,
    MbReg_SetDry = 3010, // 报警干接点开关

    MbReg_Restores = 4000,  // 恢复出厂设置
    MbReg_DualCtr = 4301,
    MbReg_MaxSize = 5000
};




#endif // MbReg_FC_H
