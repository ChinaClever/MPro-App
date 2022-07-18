#ifndef CFG_RWMAIN_H
#define CFG_RWMAIN_H

#include "cfg_readparam.h"

class Cfg_RwMain : public Cfg_ReadParam
{
    Q_OBJECT
    explicit Cfg_RwMain(QObject *parent = nullptr);
public:
    static Cfg_RwMain *bulid(QObject *parent = nullptr);
    bool readParamDefault() {return readParam(CFG_PARAM_DN);}
    bool readAlarmDefault() {return readAlarm(CFG_ALARM_DF);}
    void writeAlarmDefault();

private slots:
    bool readSettings();
};

using Cfg_ReadWrite = Cfg_RwMain;
#endif // CFG_RWMAIN_H
