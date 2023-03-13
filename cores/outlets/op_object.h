#ifndef OP_OBJECT_H
#define OP_OBJECT_H
#include "crc.h"
#include "serialport.h"
#define OP_ARRAY_SIZE 16


/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sOpIt
{
    uchar type; // 0 执行板 １　总监测板
    uchar addr;
    uchar size;
    uchar hz;
    uint vol[OP_ARRAY_SIZE]; // 电压
    uint cur[OP_ARRAY_SIZE]; // 电流
    uint pow[OP_ARRAY_SIZE]; // 有功功率
    uint ele[OP_ARRAY_SIZE]; // 电能
    uint pf[OP_ARRAY_SIZE]; // 功率因数
    uint sw[OP_ARRAY_SIZE]; // 开关状态  0 表示未启用 1 表示开
    uint activePow[OP_ARRAY_SIZE]; // 视在功率
    uint reactivePow[OP_ARRAY_SIZE]; // 无功功率
    uchar chipStatus; //  01表示执行版计量芯片模块损坏，00表示正常
    uchar version;  // 执行板软件版本号
    uchar ens[DEV_NUM];
};


class OP_Object : public SerialPort
{
public:
    explicit OP_Object(QObject *parent = nullptr);
    enum {zCmdLen=68, zRcvLen=127};

protected:
    void fillData(uchar addr);
    void loop_fillData();

private:
    bool volFaultCheck(uchar k, uchar i);
    bool curFaultCheck(uchar k, uchar i);
    void powFaultCheck(uchar k, uchar i);
    void eleFaultCheck(uchar k, uchar i);
    void relayCheck(uint &sw, uint &src_sw);

    void recoveryLog(int id, uint *cnt);
    void faultLog(int id, uint *cnt, uint value);
    bool faultCode(int id, bool f, uint *cnt, FaultCode code);
    bool dataFiltering(uint &dest, uint &src, uint max, uint min=0);

protected:
    sOpIt *mOpData=nullptr;
    sDevData *mDev=nullptr;
};

#endif // OP_OBJECT_H
