#ifndef ODBC_DEV_H
#define ODBC_DEV_H
#include "odbc_sql.h"

struct sOdbcDevIt {
    int id = -1;
    QString dev_key;
    int cascade_addr;
    int run_status;

    QString dev_type;
    QString room_name;
    QString dev_position;
    QString sw_version;
    QString ver_md;

    int dev_mode;
    int slave_num;

    int line_num;
    int loop_num;
    int output_num;

    int dev_spec;
    int rtu_addr;

    QString mac;
    QString ip_v4;
    QString ip_v6;
    QString qrcode;
};

class Odbc_Dev : public Odbc_Sql
{
public:
    Odbc_Dev();

    uint devKey(int addr);

protected:
    bool dev_createTable();
    bool dev_polls();

private:
    bool dev_poll(int addr);
    int dev_counts(const sOdbcDevIt &it);
    bool dev_update(const sOdbcDevIt &it);
    bool dev_insert(const sOdbcDevIt &it);
    void dev_params(int addr, sOdbcDevIt &it);
    bool dev_modifyItem(const sOdbcDevIt &it, const QString &fmd);

private:
    QMap<int, uint> mKeys;
};

#endif // ODBC_DEV_H
