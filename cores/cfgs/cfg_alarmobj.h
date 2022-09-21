#ifndef CFG_ALARMOBJ_H
#define CFG_ALARMOBJ_H

#include "cfg_com.h"
#include "cfg_alarmstream.h"

#define CFG_ALARM_FN    "cfg_alarm.bin"
#define CFG_ALARM_DF    "cfg_alarm.df"
#define CFG_PARAM_FN    "cfg_param.bin"
#define CFG_PARAM_DF    "cfg_param.df"


class Cfg_AlarmObj
{
public:
    explicit Cfg_AlarmObj();
    void alarm_run() {saveAlarms();}
    void writeAlarms();

protected:
    virtual void fillData()=0;
    virtual void unSequence()=0;
    bool readAlarm(const QString &fn);
    cfg::_sDevData *getDev() {return mData;}

private:
    bool saveAlarms();
    QByteArray toDataStream();
    cfg::_sDevData *deDataStream(QByteArray &array);

private:
    bool isRun;
    QFile *mFile;
    cfg::_sDevData *mData;
    Cfg_AlarmStream *mDataStream;
};

#endif // CFG_ALARMOBJ_H
