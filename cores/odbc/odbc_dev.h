#ifndef ODBC_DEV_H
#define ODBC_DEV_H
#include "odbc_uut.h"

struct sOdbcDevIt {
    int id = -1;
    uint pdu_id=0;
    QString dev_type;

    int dev_spec;
    int dev_mode;
    int line_num;
    int loop_num;
    int output_num;

    QString ip_v4;
    QString ip_v6;
    int cascade_addr;
    int slave_num;
    QString sw_version;
};

class Odbc_Dev : public Odbc_Uut
{
public:
    Odbc_Dev();

protected:
    bool dev_createTable();
    bool dev_polls();

private:
    bool dev_poll(int addr);
    int dev_counts(const sOdbcDevIt &it);
    bool dev_update(const sOdbcDevIt &it);
    bool dev_insert(const sOdbcDevIt &it);
    bool dev_params(int addr, sOdbcDevIt &it);
    bool dev_modifyItem(const sOdbcDevIt &it, const QString &fmd);
};

#endif // ODBC_DEV_H
