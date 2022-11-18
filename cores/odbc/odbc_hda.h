#ifndef ODBC_HDA_H
#define ODBC_HDA_H
#include "odbc_sql.h"

struct sOdbcHdaIt {
    int addr;
    int type;
    int topic;
    int subindex;
    double value;
};

class Odbc_Hda : public Odbc_Sql
{
public:
    Odbc_Hda();

protected:
    bool hda_createTable();
    bool hda_insert(const sOdbcHdaIt &it);

private:
    bool hda_modifyItem(const sOdbcHdaIt &it, const QString &fmd);
};

#endif // ODBC_HDA_H
