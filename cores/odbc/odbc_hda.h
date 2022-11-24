#ifndef ODBC_HDA_H
#define ODBC_HDA_H
#include "odbc_threshold.h"

struct sOdbcHdaIt {
    uchar addr;
    uchar type;
    uchar topic;
    uchar indexes;
    double value;
};

class Odbc_Hda : public Odbc_Threshold
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
