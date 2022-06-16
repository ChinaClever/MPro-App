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
    void upRelayUnit(uchar id, sRelayUnit &unit, set::_sRelayIt &it);
    void upAlarmUnit(uchar id, sAlarmUnit &unit, set::_sAlarmIt &it);
    void upObjData(uchar id, sObjData &data, set::_sObjData &obj);
    void upEnvData(uchar id, sEnvData &data, set::_sEnvData &obj);
    void upDevData(sDevData *data, set::_sDevData *obj);
};

#endif // CFG_RWFILL_H
