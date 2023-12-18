#ifndef MB_ENV_H
#define MB_ENV_H

#include "mb_group.h"

class Mb_Env : public Mb_Group
{
public:
    explicit Mb_Env(QObject *parent = nullptr);

protected:
    void env_update();
    void env_setting(ushort addr, ushort address, ushort value);

private:
    void env_dataUpdate();
    void env_alarmUpdate();
    void env_sensorUpdate();
    void env_thresholdUpdate();
    void env_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs);
};

#endif // MB_ENV_H
