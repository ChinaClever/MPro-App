#ifndef CASCADE_FILL_H
#define CASCADE_FILL_H

#include "cascade_master.h"

class Cascade_Fill : public Cascade_Master
{
    Q_OBJECT
public:
    explicit Cascade_Fill(QObject *parent = nullptr);

protected:
    void fillData(uchar addr);

private:
    void upRelayUnit(uchar id, sRelayUnit &unit, c_sRelayUnit &it);
    void upAlarmUnit(uchar id, sAlarmUnit &unit, c_sAlarmUnit &it);
    void upObjData(uchar id, sObjData &data, c_sObjData &obj);
    void upEnvData(uchar id, sEnvData &data, c_sEnvData &obj);
    void upDevData(uchar addr, sDevData *data, c_sDevData *obj);
};

#endif // CASCADE_FILL_H
