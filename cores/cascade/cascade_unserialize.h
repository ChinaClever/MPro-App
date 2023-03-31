#ifndef CASCADE_UNSERIALIZE_H
#define CASCADE_UNSERIALIZE_H

#include "cascade_updater.h"

class Cascade_Unserialize : public Cascade_Updater
{
public:
    explicit Cascade_Unserialize(QObject *parent = nullptr);

protected:
    bool unSequence(uchar addr);

private:
    void unRelayUnit(uchar id, sRelayUnit &unit, c_sRelayUnit &it);
    void unAlarmUnit(uchar id, sAlarmUnit &unit, c_sAlarmUnit &it);
    void unCabAlarmUnit(uchar id, sAlarmUnit &unit, c_sAlarmUnit &it);

    void unObjData(uchar id, sObjData &data, c_sObjData &obj);
    void unCabData(uchar id, sObjData &data, c_sObjData &obj);
    void unEnvData(uchar id, sEnvData &data, c_sEnvData &obj);
    void unDevData(sDevData *data, c_sDevData *obj);
    void unDevSize(int devSpec, uchar size, sObjData &data);
};

#endif // CASCADE_UNSERIALIZE_H
