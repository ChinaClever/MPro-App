#ifndef CASCADE_MASTER_H
#define CASCADE_MASTER_H

#include "cascade_unserialize.h"

class Cascade_Master : public Cascade_Unserialize
{
    Q_OBJECT
public:
    explicit Cascade_Master(QObject *parent = nullptr);

    void masterReadDevs();
    bool masterSetUut(uchar addr, const sUutInfo &uut);
    bool masterSetAlarm(const sSetAlarmUnit &unit);
    bool masterRelayCtrl(uchar addr, uchar id, uchar on);
    bool masterDelaySet(int addr, int id, uchar delay);
    bool masterSwModeSet(int addr, int id, uchar type);
    bool masterOpNameSet(int addr, int id, const QString &name);

private:
    bool masterRead(uchar addr);
    void setEndisable(int addr, bool ret, uchar &v);
};

#endif // CASCADE_MASTER_H
