#ifndef FTP_DATA_H
#define FTP_DATA_H
#include "ftp_csv.h"

class Ftp_Data : public Ftp_Csv
{
public:
    Ftp_Data();
    void upDevData();

private:
    void upRelayUnit(int id, const sRelayUnit &relay, QStringList &values);
    void upObjData(int id, const QString &name, const sObjData *obj, QStringList &values);

    void upLineData(int addr);
    void upLoopData(int addr);
    void upDualData(int addr);
    void upTotalData(int addr);
    void upOutletData(int addr);
};

#endif // FTP_DATA_H
