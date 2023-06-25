#ifndef CFG_SERVICE_H
#define CFG_SERVICE_H

#include "cfg_devparam.h"

class Cfg_Service : public Cfg_devParam
{
public:
    explicit Cfg_Service();
    void readCfgParams();

private:
    void log();
    void ssh();
    void ntp();
    void web();
    void rpc();
    void aiot();
    void smtp();
    void push();
    void snmp();
    void mqtt();
    void amqp();
    void odbc();
    void ldap();
    void login();
    void redis();
    void script();
    void radius();
    void modbus();
    void syslog();
    void sercret();
    void whiteList();
    void dualName();
    void groupName();
    void outputName();

private:
     Cfg_Com *mCfg;
};

#endif // CFG_SERVICE_H
