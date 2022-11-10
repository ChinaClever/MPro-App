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

    QVariant net_diagnoseCfg(int fc);
    bool net_diagnoseSet(int fc, const QVariant &v);

private:
    sNetDgsIt mNetDgs;
};

#endif // SET_DIAGNOSE_H
