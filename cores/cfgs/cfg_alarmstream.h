#ifndef CFG_ALARMSTREAM_H
#define CFG_ALARMSTREAM_H
#include "alarm_log.h"
#include <QDataStream>

namespace cfg {
struct _sAlarmIt
{
    uint rated;
    uchar en;
    uint min; // 最小值
    uint max; // 最大值

    uint crMin; // 最小值
    uint crMax; // 最大值
    uint reserve; // 预留
};

struct _sRelayIt
{
    uchar en;
    uchar offAlarm;
    uchar powerUpDelay;
    uchar resetDelay;
    uchar overrunOff;
    uchar timingEn;
    uchar reserve; // 预留
    char timingOn[NAME_SIZE];
    char timingOff[NAME_SIZE];
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

    uchar groupSize;
    _sObjData group[LOOP_NUM]; // 回路数据

    uchar dualSize;
    _sObjData dual[OUTPUT_NUM]; //双电源数据

    uchar outputSize;
    _sObjData output[OUTPUT_NUM]; //位数据

    uchar envSize;
    _sEnvData env[SENOR_NUM]; // 环境数据
    sTgObjData tg; // 回路数据
};
}

class Cfg_AlarmStream
{
public:
    explicit Cfg_AlarmStream(cfg::_sDevData *data);
    friend QDataStream& operator>>(QDataStream&, Cfg_AlarmStream&);
    friend QDataStream& operator<<(QDataStream&, Cfg_AlarmStream&);

private:
    cfg::_sDevData *mDevData;
};

#endif // CFG_ALARMSTREAM_H
