#ifndef DATA_RELAY_H
#define DATA_RELAY_H
#include "data_object.h"

class Data_Relay : public Data_Object
{
public:
    Data_Relay();

protected:
    bool relayOverrunOff(const sObjData &obj, int id);
    int relayTiming(const sObjData &obj, int id);

private:
    bool relayTimingCheck(const char (*str)[NAME_SIZE], int id);
};

#endif // DATA_RELAY_H
