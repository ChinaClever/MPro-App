#ifndef CFG_RWMAIN_H
#define CFG_RWMAIN_H

#include "cfg_readparam.h"

class Cfg_RwMain : public Cfg_ReadParam
{    
    explicit Cfg_RwMain();
public:
    static Cfg_RwMain *bulid();
    //bool readParamDefault() {return readParam(CFG_PARAM_DF);}
    bool readAlarmDefault() {return readAlarm(CFG_ALARM_DF);}
    void writeAlarmDefault();

private:
    bool readSettings();
};

using Cfg_ReadWrite = Cfg_RwMain;
#endif // CFG_RWMAIN_H
