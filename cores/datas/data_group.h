#ifndef DATA_GROUP_H
#define DATA_GROUP_H

#include "data_line.h"

class Data_Group : public Data_Line
{
public:
    explicit Data_Group();

    QList<int> outletByGroup(int id, int addr=0);
protected:
    void groupWork();

private:
    void setGroupSize();
    void disGroupAlarm(int id);
};

#endif // DATA_GROUP_H
