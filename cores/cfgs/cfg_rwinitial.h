#ifndef CFG_RWINITIAL_H
#define CFG_RWINITIAL_H

#include "cfg_rwunserialize.h"

class Cfg_RwInitial : public Cfg_RwUnserialize
{
public:
    explicit Cfg_RwInitial(QObject *parent = nullptr);
    bool initialData();
    bool initialParam();
private:
    void initDevInfo(sDevInfo &it);
    void setAlarmUnit(sAlarmUnit &it, uchar size, uint rated);
    void initObjData(sObjData &it, uchar size, uint curRated);

    void setVolAlarm(sAlarmUnit &it, uchar size, uint rated);
    void setTgVol(sTgUnit &it, uint rated);
    void setTgUnit(sTgUnit &it, uint rated);
    void initTgObjData(sTgObjData &it);

    void initEnvData(sEnvData &it);
    void initDevData(sDevData *dev);
    void initRelayUnit(sRelayUnit &it, uchar size);
};

#endif // CFG_RWINITIAL_H
