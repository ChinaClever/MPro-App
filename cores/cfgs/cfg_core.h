#ifndef CFG_CORE_H
#define CFG_CORE_H

#include "cfg_service.h"

class Cfg_Core : public Cfg_Service
{    
    explicit Cfg_Core();
public:
    static Cfg_Core *bulid();
    bool readAlarmDefault() {return readAlarm(CFG_ALARM_DF);}
    void writeAlarmDefault();

private:
    bool readSettings();
};

#endif // CFG_CORE_H
