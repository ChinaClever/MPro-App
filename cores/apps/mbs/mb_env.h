#ifndef MB_ENV_H
#define MB_ENV_H

#include "mb_loop.h"

class Mb_Env : public Mb_Loop
{
    Q_OBJECT
public:
    explicit Mb_Env(QObject *parent = nullptr);

signals:

};

#endif // MB_ENV_H
