#ifndef MB_REGS_H
#define MB_REGS_H
#include "modbus_slavertu.h"
#include "set_relay.h"

enum eMbRegs {
    MbReg_Factory   =   0,     // 生产厂家
    MbReg_Model     =   4,    // 型号 例：HVPDU63-2-32(01)
    MbReg_Hw       =   20,    // A-Z
    MbReg_Sw       =   21,    // 100-999
    MbReg_Version  =   22,    // 0x100-0x999 0x100-0x999
    MbReg_CompileTime  = 24,  // 软件编译时间 YYYYMMDD（字符串）

    MbReg_Rateds  = 28,  // 额定电压 额定电流 额定频率
    MbReg_OutputType = 31, // 输出位数 输出位开关功能 输出位电流计量功能 输出位电度计量功能

    MbReg_Totals = 35, // 电压、电流、有功功率、无功功率、功率因数、总有功电度
    MbReg_Hz   = 42,  // 频率 4500-6500

    MbReg_Lines = 43, // 相电压、相电流、相有功功率    
    MbReg_Outputs = 58, // 输出位参数

    MbReg_PhaseNum = 346, // 1：单相，2：双火线；3：三相；4：三火线
    MbReg_Classify = 347, // 1：主路监测型，2：开关型

    MbReg_Loops = 400,  // 回路参数
    MbReg_Envs = 500, // 环境参数

    MbReg_Settings = 1000,    // 配置参数
    MbReg_SetLine = 1003,
    MbReg_SetOutput = 1011,
    MbReg_SetEnv = 1107,
    MbReg_Alarms = 2000,    // 警报：01H功能码读

    MbReg_Controls = 3000,  // 控制量
    MbReg_Restores = 4000,  // 恢复出厂设置
};




#endif // MbReg_FC_H
