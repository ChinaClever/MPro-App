#ifndef SET_RWSTREAM_H
#define SET_RWSTREAM_H
#include "alarm_log.h"
#include <QDataStream>

namespace set {
struct _sAlarmIt
{
    uint rated;
    ushort min; // 最小值
    ushort max; // 最大值

    ushort crMin; // 最小值
    ushort crMax; // 最大值
};

struct _sRelayIt
{
    uchar sw;
    uchar state;
    uchar delay;
};

struct _sObjData {
    char name[NAME_SIZE];
    _sAlarmIt vol;
    _sAlarmIt cur;
    _sAlarmIt pow;
    _sRelayIt relay;
};


struct _sEnvData
{
    _sAlarmIt tem; // 温度
    _sAlarmIt hum; // 湿度
};

struct _sDevData
{
    _sDevData() {version=1;}
    uchar version;
    uchar lineSize;
    _sObjData line[LINE_NUM]; // 相数据

    uchar loopSize;
    _sObjData loop[LOOP_NUM]; // 回路数据

    uchar outputSize;
    _sObjData output[OUTPUT_NUM]; //位数据

    uchar envSize;
    _sEnvData env[SENOR_NUM]; // 环境数据
    sTgObjData tg; // 回路数据
    sDevInfo info;
    sDevLogin login;
};
}

class Set_RwStream
{
public:
    explicit Set_RwStream(set::_sDevData *data);
    friend QDataStream& operator>>(QDataStream&, Set_RwStream&);
    friend QDataStream& operator<<(QDataStream&, Set_RwStream&);

private:
    set::_sDevData *mDevData;
};

#endif // SET_RWSTREAM_H
