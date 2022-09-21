#ifndef _CFG_ALARMUNSERIALIZE_H
#define _CFG_ALARMUNSERIALIZE_H

#include "cfg_alarmfill.h"

class Cfg_AlarmUnserialize : public Cfg_AlarmFill
{
public:
    explicit Cfg_AlarmUnserialize();

protected:
    void unSequence() override ;

private:
    void unRelayUnit(uchar id, sRelayUnit &unit, cfg::_sRelayIt &it);
    void unAlarmUnit(uchar id, sAlarmUnit &unit, cfg::_sAlarmIt &it);
    void unObjData(uchar id, sObjData &data, cfg::_sObjData &obj);
    void unEnvData(uchar id, sEnvData &data, cfg::_sEnvData &obj);
    void unDevData(sDevData *data, cfg::_sDevData *obj);
    void unDevSize(uchar size, sObjData &data);
};

#endif // cfg_UNSERIALIZE_H
