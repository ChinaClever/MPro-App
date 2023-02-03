#ifndef MB_ENV_H
#define MB_ENV_H

#include "mb_loop.h"

class Mb_Env : public Mb_Loop
{
public:
    explicit Mb_Env(QObject *parent = nullptr);

protected:
    void env_update();

private:
    void env_dataUpdate();
    void env_alarmUpdate();
    void env_sensorUpdate();
    void env_thresholdUpdate();
    void env_thresholdObj(const sAlarmUnit &unit, int id, vshort &vs);
};

#endif // MB_ENV_H
