#ifndef PROCSTART_H
#define PROCSTART_H

#include "procruning.h"

class ProcStart : public ProcRuning
{
public:
    explicit ProcStart();

    void startCore();
    void startLcd();
    void proc_log(const QString &arg);

private:
    void proc_start(const QString &app, const QString &arg="");
};

#endif // PROCSTART_H
