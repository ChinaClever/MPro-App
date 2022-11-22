#ifndef ODBC_THRESHOLD_H
#define ODBC_THRESHOLD_H
#include "odbc_dev.h"

struct sOdbcThIt {
    uchar addr;
    uchar type;
    uchar topic;
    uchar subtopic;
    uchar indexes;
    float value;
};

class Odbc_Threshold : public Odbc_Dev
{
public:
    Odbc_Threshold();

protected:
    bool th_createTable();
    bool th_poll(const sOdbcThIt &it);

private:
    int th_counts(const sOdbcThIt &it);
    bool th_update(const sOdbcThIt &it);
    bool th_insert(const sOdbcThIt &it);
    bool th_modifyItem(const sOdbcThIt &it, const QString &cmd);
};

#endif // ODBC_THRESHOLD_H
