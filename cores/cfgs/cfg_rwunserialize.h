#ifndef _CFG_RWUNSERIALIZE_H
#define _CFG_RWUNSERIALIZE_H

#include "cfg_rwfill.h"

class Cfg_RwUnserialize : public Cfg_RwFill
{
public:
    explicit Cfg_RwUnserialize(QObject *parent = nullptr);

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
