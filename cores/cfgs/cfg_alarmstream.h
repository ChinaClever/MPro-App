#ifndef CFG_ALARMSTREAM_H
#define CFG_ALARMSTREAM_H
#include "alarm_log.h"
#include <QDataStream>

namespace cfg {
struct _sAlarmIt
{
    uchar en;
    uchar size;
    uint rated;
    uint min; // 最小值
    uint max; // 最大值
    uchar hda; // 数据记录开关

    uint crMin; // 最小值
    uint crMax; // 最大值
    uint peakMax; // 峰值
    uint peakStamp; // 峰值的时间戳
    uint reserve[6]; // 预留
};

struct _sRelayIt
{
    uchar size;
    uint cnt; //继电器控制次数
    uint maxCnt; // 继电器最大控制次数
    uchar disabled;
    uchar offAlarm;
    uchar powerUpDelay;
    uchar resetDelay;
    uchar overrunOff;
    uchar timingEn;
    uchar lifeEn;
    uchar reserve[6]; // 预留
    char timingOn[NAME_SIZE];
    char timingOff[NAME_SIZE];
};

struct _sObjData {
    char name[NAME_SIZE];
    _sAlarmIt vol;
    _sAlarmIt cur;
    _sAlarmIt pow;
    _sRelayIt relay;
    uint hdaEle;
    uint reserve[6]; // 预留
};


struct _sEnvData
{
    _sAlarmIt tem; // 温度
    _sAlarmIt hum; // 湿度
    uint reserve[6]; // 预留
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
    _sObjData group[GROUP_NUM]; // 回路数据

    uchar dualSize;
    _sObjData dual[OUTPUT_NUM]; //双电源数据

    uchar cabLineSize;
    _sObjData cabLine[LINE_NUM]; //机柜相

    uchar cabLoopSize;
    _sObjData cabLoop[LOOP_NUM]; //机柜回路

    uchar outputSize;
    _sObjData output[OUTPUT_NUM]; //位数据

    uchar envSize;
    _sEnvData env[SENOR_NUM]; // 环境数据
    sTgObjData tg; // 回路数据
    sTgObjData cabTg; // 回路数据
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
