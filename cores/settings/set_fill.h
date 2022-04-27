#ifndef SET_FILL_H
#define SET_FILL_H

#include "set_object.h"

class Set_Fill : public Set_Object
{
    Q_OBJECT
public:
    explicit Set_Fill(QObject *parent = nullptr);

protected:
    void fillData() override ;

private:
    void upRelayUnit(uchar id, sRelayUnit &unit, set::_sRelayIt &it);
    void upAlarmUnit(uchar id, sAlarmUnit &unit, set::_sAlarmIt &it);
    void upObjData(uchar id, sObjData &data, set::_sObjData &obj);
    void upEnvData(uchar id, sEnvData &data, set::_sEnvData &obj);
    void upDevData(sDevData *data, set::_sDevData *obj);
};

#endif // SET_FILL_H
