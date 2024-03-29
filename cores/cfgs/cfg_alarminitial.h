#ifndef CFG_ALARMINITIAL_H
#define CFG_ALARMINITIAL_H

#include "cfg_alarmunserialize.h"

class Cfg_AlarmInitial : public Cfg_AlarmUnserialize
{
public:
    explicit Cfg_AlarmInitial();
    bool initialData();

private:
    void setAlarmUnit(sAlarmUnit &it, uchar size, uint rated);
    void initObjData(sObjData &it, uchar size, uint curRated);

    void setVolAlarm(sAlarmUnit &it, uchar size, uint rated);
    void setTgVol(sTgUnit &it, uint rated);
    void setTgUnit(sTgUnit &it, uint rated);
    void initTgObjData(sTgObjData &it);

    void initEnvData(sEnvData &it);
    void initDevData(sDevData *dev);
    void initObjHda(sObjData &it, uchar size);
    void initRelayUnit(sRelayUnit &it, uchar size);
};

#endif // CFG_ALARMINITIAL_H
