#ifndef SET_ALARM_H
#define SET_ALARM_H

#include "cfg_core.h"
#include "log_core.h"

class Set_Alarm : public Alarm_Object
{
public:
    explicit Set_Alarm();

protected:
    bool setAlarm(sDataItem &unit);
    void setAlarmLog(sDataItem &unit);
    QString opSrc(uchar addr, uchar txType);

private:
    QString opContent(const sDataItem &index);
    void oplog(const sDataItem &it);
};

#endif // SET_ALARM_H
