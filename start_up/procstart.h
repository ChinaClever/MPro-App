#ifndef PROCSTART_H
#define PROCSTART_H
#define SUPPORT_C 1
#include "/home/lzy/work/NPDU/cores/commons/cms/datapacket.h"
#include <QtCore>

class ProcStart
{
public:
    explicit ProcStart();

    void mdelay(int msec);
    void proc_log(const QString &arg);
    static bool proc_isRun(const QString &p);
    void proc_start(sRunTime &proc, const QString &app);

private:
    void proc_time(sRunTime &proc);
    void proc_md5(sRunTime &proc, const QString &fn);
    QString md5(const QString &fn);
};

#endif // PROCSTART_H
