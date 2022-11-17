#ifndef ODBC_CORE_H
#define ODBC_CORE_H
#include "odbc_data.h"

class Odbc_Core : public Odbc_Data
{
    Odbc_Core();
public:
    static Odbc_Core *bulid();
    void run();
};

#endif // ODBC_CORE_H
