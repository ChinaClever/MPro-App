#ifndef MB_OUTPUTS_H
#define MB_OUTPUTS_H

#include "mb_envs.h"

class Mb_Outputs : public Mb_Envs
{
    Q_OBJECT
public:
    explicit Mb_Outputs(QObject *parent = nullptr);

signals:

};

#endif // MB_OUTPUTS_H
