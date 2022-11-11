#ifndef SET_DIAGNOSE_H
#define SET_DIAGNOSE_H
#include "set_maintain.h"

struct sNetDgsIt
{
    sNetDgsIt() {}
    QString pingHost;
    int pingCnt = 1;

    QString routeHost;
    int timeout = 1;
};

class Set_Diagnose : public Set_Maintain
{
public:
    Set_Diagnose();

    QString downDiagnostics();
    QVariant net_diagnoseCfg(int fc);
    bool net_diagnoseSet(int fc, const QVariant &v);

private:
    void diascmds();
    void diasCopy();
    QString diasZip();
    void sys_cmd(const QString &cmd, const QString &fn);

private:
    sNetDgsIt mNetDgs;
};

#endif // SET_DIAGNOSE_H
