#ifndef AIOT_LINE_H
#define AIOT_LINE_H
#include "aiot_total.h"

class Aiot_Line : public Aiot_Total
{
public:
    Aiot_Line();
    void linePost();

private:
    void lineVols();
    void lineActivePowers();
};

#endif // AIOT_LINE_H
