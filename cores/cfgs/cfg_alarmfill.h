#ifndef CFG_ALARMFILL_H
#define CFG_ALARMFILL_H

#include "cfg_alarmobj.h"

class Cfg_AlarmFill : public Cfg_AlarmObj
{
public:
    explicit Cfg_AlarmFill();

protected:
    void fillData() override ;

private:
    void upRelayUnit(uchar id, sRelayUnit &unit, cfg::_sRelayIt &it);
    void upAlarmUnit(uchar id, sAlarmUnit &unit, cfg::_sAlarmIt &it);
    void upObjData(uchar id, sObjData &data, cfg::_sObjData &obj);
    void upEnvData(uchar id, sEnvData &data, cfg::_sEnvData &obj);
    void upDevData(sDevData *data, cfg::_sDevData *obj);
};

#endif // CFG_ALARMFILL_H
