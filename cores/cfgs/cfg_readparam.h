#ifndef CFG_READPARAM_H
#define CFG_READPARAM_H

#include "cfg_rwinitial.h"

class Cfg_ReadParam : public Cfg_RwInitial
{
public:
    explicit Cfg_ReadParam(QObject *parent = nullptr);
    void readCfgParams();

private:
    void ntp();
    void web();
    void rpc();
    void smtp();
    void push();
    void snmp();
    void mqtt();
    void login();
    void modbus();
    void sercret();
    void dualName();
    void groupName();
    void outputName();

private:
     Cfg_Com *mCfg;
};

#endif // CFG_READPARAM_H
