#ifndef SET_RWFILL_H
#define SET_RWFILL_H

#include "set_rwobj.h"

class Set_RwFill : public Set_RwObj
{
public:
    explicit Set_RwFill(QObject *parent = nullptr);

protected:
    void fillData() override ;

private:
    void upRelayUnit(uchar id, sRelayUnit &unit, set::_sRelayIt &it);
    void upAlarmUnit(uchar id, sAlarmUnit &unit, set::_sAlarmIt &it);
    void upObjData(uchar id, sObjData &data, set::_sObjData &obj);
    void upEnvData(uchar id, sEnvData &data, set::_sEnvData &obj);
    void upDevData(sDevData *data, set::_sDevData *obj);
};

#endif // SET_RWFILL_H
