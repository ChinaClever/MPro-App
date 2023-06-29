#ifndef CFG_ALARMOBJ_H
#define CFG_ALARMOBJ_H

#include "cfg_com.h"
#include "cfg_alarmstream.h"

#define CFG_ALARM_FN    "alarm.conf"
#define CFG_ALARM_DF    "alarm.cfg"
#define CFG_RELAY_FN    "relay.conf"


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
    void initAlarms();
    bool saveAlarms();
    void saveRelayCnt();
    void readRelayCnt();
    QByteArray toDataStream();
    cfg::_sDevData *deDataStream(QByteArray &array);

private:
    bool isRun;
    cfg::_sDevData *mData;
    Cfg_AlarmStream *mDataStream;
};

#endif // CFG_ALARMOBJ_H
