#ifndef ODBC_EVENT_H
#define ODBC_EVENT_H
#include "odbc_hda.h"

class Odbc_Event : public Odbc_Hda
{
public:
    Odbc_Event();

    bool event_createTable();
};

#endif // ODBC_EVENT_H
