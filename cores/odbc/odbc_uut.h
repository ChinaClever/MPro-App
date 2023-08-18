#ifndef ODBC_UUT_H
#define ODBC_UUT_H
#include "odbc_index.h"

struct sOdbcUutIt {
    int id = -1;
    uint pdu_id=0;
    QString room_name;
    QString dev_position;
    QString dev_type;
    QString dev_name;
    QString dev_sn;
    QString qrcode;
};

class Odbc_Uut : public Odbc_Index
{
public:
    Odbc_Uut();

protected:
    bool uut_createTable();
    bool uut_polls();

private:
    bool uut_insert(const sOdbcUutIt &it);
    bool uut_modifyItem(const sOdbcUutIt &item, const QString &cmd);
    bool uut_params(int addr, sOdbcUutIt &it);
    bool uut_update(const sOdbcUutIt &it);
    int uut_counts(const sOdbcUutIt &it);
    bool uut_poll(int addr);

};

#endif // ODBC_UUT_H
