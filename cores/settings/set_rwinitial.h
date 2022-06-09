#ifndef SET_RWINITIAL_H
#define SET_RWINITIAL_H

#include "set_rwunserialize.h"

class Set_RwInitial : public Set_RwUnserialize
{
public:
    explicit Set_RwInitial(QObject *parent = nullptr);
    bool initialData();

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

#endif // SET_RWINITIAL_H
