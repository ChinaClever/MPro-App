#ifndef _SET_UNSERIALIZE_H
#define _SET_UNSERIALIZE_H

#include "set_fill.h"

class Set_Unserialize : public Set_Fill
{
    Q_OBJECT
    explicit Set_Unserialize(QObject *parent = nullptr);
public:
     static Set_Unserialize *bulid(QObject *parent = nullptr);

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
using Set_readWrite = Set_Unserialize;
#endif // SET_UNSERIALIZE_H
