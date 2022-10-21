#ifndef MB_ENV_H
#define MB_ENV_H

#include "mb_loop.h"

class Mb_Env : public Mb_Loop
{
public:
    explicit Mb_Env(QObject *parent = nullptr);

protected:
    void mbEnvUpdate();

private:
    void upEnvData();
    void upEnvThreshold();
};

#endif // MB_ENV_H
