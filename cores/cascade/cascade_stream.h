#ifndef CASCADE_STREAM_H
#define CASCADE_STREAM_H

#include "set_core.h"
#include <QDataStream>

namespace cascade {
struct _sAlarmUnit : set::_sAlarmIt
{
    uint value;
};

struct _sRelayUnit : set::_sRelayIt
{
    uchar sw; // 开关状态 0 表示未启用
};

struct _sObjData {
    uchar id;
    char name[NAME_SIZE];
    _sAlarmUnit vol;
    _sAlarmUnit cur;
    _sAlarmUnit pow;
    _sRelayUnit relay;

    uint ele; // 电能
    ushort pf; // 功率因数
    ushort ratedCur;
    uint activePow;
    uint reactivePow;
};


struct _sEnvData
{
    uchar type_index;
    _sAlarmUnit tem; // 温度
    _sAlarmUnit hum; // 湿度
    uchar door; // 门禁
    uchar water; // 水浸
    uchar smoke; // 烟雾
};

struct _sDevData
{
    _sDevData() {id=0; version=1;}
    uchar version;

    uchar id;  // 设备号
    sDevInfo info;
    sUutInfo uut;

    uchar lineSize;
    _sObjData line[LINE_NUM]; // 相数据

    uchar loopSize;
    _sObjData loop[LOOP_NUM]; // 回路数据

    uchar outputSize;
    _sObjData output[OUTPUT_NUM]; //位数据

    uchar envSize;
    _sEnvData env[SENOR_NUM]; // 环境数据
    sRtuCount rtuCount; // 传输情况
    sTgObjData tg; // 回路数据

    uchar lps; // 防雷开关
    uchar dc; // 交直流标志位
    uchar hz; // 电压频率
    ushort br;  // 00	表示波特率9600(00默认9600，01为4800，02为9600，03为19200，04为38400)
};

struct _sFrame {
    _sFrame(){srcAddr=0;}
    uchar srcAddr;
    uchar dstAddr;

    uchar fc;
    ushort len;
    QByteArray data;
};
}

using c_sRelayUnit = cascade::_sRelayUnit;
using c_sAlarmUnit = cascade::_sAlarmUnit;
using c_sObjData = cascade::_sObjData;
using c_sEnvData = cascade::_sEnvData;
using c_sDevData = cascade::_sDevData;
using c_sFrame = cascade::_sFrame;

class Cascade_Stream
{
public:
    explicit Cascade_Stream(c_sDevData *data);
    Cascade_Stream& operator=(const Cascade_Stream &other);
    friend QDataStream& operator>>(QDataStream&, Cascade_Stream&);
    friend QDataStream& operator<<(QDataStream&, Cascade_Stream&);

private:
    c_sDevData *mDevData;
};
using c_DataStream = Cascade_Stream;

#endif // CASCADE_STREAM_H
