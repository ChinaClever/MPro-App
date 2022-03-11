#ifndef MB_OUTPUT_H
#define MB_OUTPUT_H

#include "mb_env.h"

class Mb_Output : public Mb_Env
{
    Q_OBJECT
public:
    explicit Mb_Output(QObject *parent = nullptr);

signals:

};

#endif // MB_OUTPUT_H
