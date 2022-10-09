#ifndef PROCRUNING_H
#define PROCRUNING_H
#include <QtCore>

class ProcRuning
{
public:
    static bool isRun(const char *procname);

private:
    static int isRunning(const char *procname);
};
void cm_mdelay(int msec);

#endif // PROCRUNING_H
