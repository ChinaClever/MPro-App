#ifndef CASCADE_MASTER_H
#define CASCADE_MASTER_H

#include "cascade_unserialize.h"

class Cascade_Master : public Cascade_Unserialize
{
    Q_OBJECT
public:
    explicit Cascade_Master(QObject *parent = nullptr);

    void masterReadDevs();
    bool masterRead(uchar addr);
    bool masterSetAlarm(const sSetAlarmUnit &unit);
    bool masterRelayCtrl(uchar addr, uchar id, uchar on);
    bool masterDelaySet(int addr, int id, uchar type, uchar delay);
};

#endif // CASCADE_MASTER_H
