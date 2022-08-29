#ifndef OP_OBJECT_H
#define OP_OBJECT_H

#include "serialport.h"
#define OP_ARRAY_SIZE 16


/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sOpIt
{
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
    Q_OBJECT
public:
    explicit OP_Object(QObject *parent = nullptr);

protected:
    void fillData(uchar addr);
private:
    bool appendLog(int addr);
    bool dataFiltering(uint &dest, uint &src, uint max, uint min=0);

protected:
    sOpIt *mOpData;
    sDevData *mDev;
};

#endif // OP_OBJECT_H
