#ifndef MPDURTU_ENV_H
#define MPDURTU_ENV_H

#include "mpdurtu_group.h"

class Mpdurtu_Env : public Mpdurtu_Group
{
public:
    explicit Mpdurtu_Env(QObject *parent = nullptr);
    void env_Update();
    void env_setting(ushort addr, ushort address, ushort value);

private:
    void env_BaseUpdate(sEnvData *obj, uint v[], int startReg);
    void env_BaseUpdateAlram(uint v[], int startReg);

};

#endif // MPDURTU_ENV_H
