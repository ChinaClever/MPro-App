#ifndef CFG_READPARAM_H
#define CFG_READPARAM_H

#include "cfg_rwinitial.h"

class Cfg_ReadParam : public Cfg_RwInitial
{
public:
    explicit Cfg_ReadParam(QObject *parent = nullptr);
    void readCfgParams();

private:
    void rpc();
    void snmp();
    void login();
    void modbus();
    void readUut();
    void outputName();

private:
     Cfg_Obj *mCfg;
};

#endif // CFG_READPARAM_H
