#ifndef MPDURTU_REGS_H
#define MPDURTU_REGS_H
#include "modbus_slavertu.h"
#include "set_core.h"
#include "op_core.h"

enum eMRegs {
    MReg_Type   =   0,     // PDU类型
    MReg_Ip     =   1,    // IP地址
    MReg_MAC     =   5,    // MAC地址
    MReg_OutputNum =   11,    // 输出位个数

    MReg_OutputRelay = 0x100,
    MReg_LineCurValue = 0x200, // 相电流
    MReg_LineCurMin = 0x300, // 相电流最小值
    MReg_LineCurMax = 0x400, // 相电流最大值
    MReg_LineCurCrMin = 0x500, // 相电流下临界值
    MReg_LineCurCrMax = 0x600, // 相电流上临界值

    MReg_LineVolValue = 0x700, // 相电压
    MReg_LineVolMin = 0x800, // 相电压最小值
    MReg_LineVolMax = 0x900, // 相电压最大值
    MReg_LineVolCrMin = 0x1000, // 相电压下临界值
    MReg_LineVolCrMax = 0x1100, // 相电压上临界值
    MReg_LinePower = 0x1200,     // 相功率
    MReg_LinePowerFactor = 0x1300,     // 相功率因数
    MReg_LineEle = 0x1400,     // 总电能

    MReg_LoopCurValue = 0x1500, // 回路电流
    MReg_LoopCurMin = 0x1600, // 回路电流最小值
    MReg_LoopCurMax = 0x1700, // 回路电流最大值
    MReg_LoopCurCrMin = 0x1800, // 回路电流下临界值
    MReg_LoopCurCrMax = 0x1900, // 回路电流上临界值

    MReg_LoopRelay = 0x2000, // 回路断路器
    MReg_LoopEle = 0x2100, // 回路电能

    MReg_OutputCurValue = 0x2200, // 输出位电流
    MReg_OutputCurMin = 0x2300, // 输出位电流最小值
    MReg_OutputCurMax = 0x2400, // 输出位电流最大值
    MReg_OutputCurCrMin = 0x2500, // 输出位电流下临界值
    MReg_OutputCurCrMax = 0x2600, // 输出位电流上临界值
    MReg_OutputPowerFactor = 0x2700,  // 输出位功率因数
    MReg_OutputEle = 0x2800,  // 输出位电能

    MReg_Tem = 0x2900,
    MReg_TemMin = 0x3000,
    MReg_TemMax = 0x3100,
    MReg_TemCrMin = 0x3200,
    MReg_TemCrMax = 0x3300,
    MReg_Hum = 0x3400,
    MReg_HumMin = 0x3500,
    MReg_HumMax = 0x3600,
    MReg_HumCrMin = 0x3700,
    MReg_HumCrMax = 0x3800,

    MReg_Door = 0x3900,
    MReg_Water = 0x4000,
    MReg_Smoke = 0x4100,
    MReg_MaxSize = 0x4101
};




#endif // MPDURTUReg_FC_H
