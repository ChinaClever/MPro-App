#ifndef PROCSTART_H
#define PROCSTART_H

#define SUPPORT_C 1
#include "/home/lzy/work/NPDU/cores/commons/cms/datapacket.h"
#include "procruning.h"

class ProcStart : public ProcRuning
{
public:
    explicit ProcStart();

    void core_start();
    void lcd_start();
    void ota_start();


    void proc_log(const QString &arg);
    void proc_start(sRunTime &proc, const QString &app);

private:
    void proc_time(sRunTime &proc);
    void proc_md5(sRunTime &proc, const QString &fn);
    QString md5(const QString &fn);
};

#endif // PROCSTART_H
