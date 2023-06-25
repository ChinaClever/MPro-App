#ifndef AIOT_LOOP_H
#define AIOT_LOOP_H
#include "aiot_line.h"

class Aiot_Loop : public Aiot_Line
{
public:
    Aiot_Loop();
    void loopPost();

private:
    void loopBreakers();
    void loopActivePowers();
};

#endif // AIOT_LOOP_H
