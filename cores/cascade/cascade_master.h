#ifndef CASCADE_MASTER_H
#define CASCADE_MASTER_H

#include "cascade_unserialize.h"

class Cascade_Master : public Cascade_Unserialize
{
    Q_OBJECT
public:
    explicit Cascade_Master(QObject *parent = nullptr);

    void masterReadDevs();
    bool masterSet(const sDataItem &unit);
    bool masterSetString(const sStrItem &it);

private:
    bool masterRead(uchar addr);
    void setEndisable(int addr, bool ret, uchar &v);
};

#endif // CASCADE_MASTER_H
