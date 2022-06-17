#ifndef CFG_RWFILL_H
#define CFG_RWFILL_H

#include "cfg_rwobj.h"

class Cfg_RwFill : public Cfg_RwObj
{
public:
    explicit Cfg_RwFill(QObject *parent = nullptr);

protected:
    void fillData() override ;

private:
    void upRelayUnit(uchar id, sRelayUnit &unit, cfg::_sRelayIt &it);
    void upAlarmUnit(uchar id, sAlarmUnit &unit, cfg::_sAlarmIt &it);
    void upObjData(uchar id, sObjData &data, cfg::_sObjData &obj);
    void upEnvData(uchar id, sEnvData &data, cfg::_sEnvData &obj);
    void upDevData(sDevData *data, cfg::_sDevData *obj);
};

#endif // CFG_RWFILL_H
