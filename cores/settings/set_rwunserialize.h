#ifndef _SET_RWUNSERIALIZE_H
#define _SET_RWUNSERIALIZE_H

#include "set_rwfill.h"

class Set_RwUnserialize : public Set_RwFill
{
public:
    explicit Set_RwUnserialize(QObject *parent = nullptr);

protected:
    void unSequence() override ;

private:
    void unRelayUnit(uchar id, sRelayUnit &unit, set::_sRelayIt &it);
    void unAlarmUnit(uchar id, sAlarmUnit &unit, set::_sAlarmIt &it);
    void unObjData(uchar id, sObjData &data, set::_sObjData &obj);
    void unEnvData(uchar id, sEnvData &data, set::_sEnvData &obj);
    void unDevData(sDevData *data, set::_sDevData *obj);
    void unDevSize(uchar size, sObjData &data);
};

#endif // SET_UNSERIALIZE_H
