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
    void unRelayUnit(uchar id, sRelayUnit &unit, cfg::_sRelayIt &it);
    void unAlarmUnit(uchar id, sAlarmUnit &unit, cfg::_sAlarmIt &it);
    void unObjData(uchar id, sObjData &data, cfg::_sObjData &obj);
    void unEnvData(uchar id, sEnvData &data, cfg::_sEnvData &obj);
    void unDevData(sDevData *data, cfg::_sDevData *obj);
    void unDevSize(uchar size, sObjData &data);
};

#endif // cfg_UNSERIALIZE_H
