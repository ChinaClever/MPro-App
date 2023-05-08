#ifndef CFG_CORE_H
#define CFG_CORE_H

#include "cfg_service.h"

class Cfg_Core : public Cfg_Service
{    
    explicit Cfg_Core();
public:
    static Cfg_Core *bulid();
    void writeAlarmDefault();

private:
    bool readSettings();
    void slaveAlarmDefault();
};

#endif // CFG_CORE_H
