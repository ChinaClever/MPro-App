#ifndef FTP_XLSX_H
#define FTP_XLSX_H
#include <QtCore>

class Ftp_Xlsx
{
    Ftp_Xlsx();
public:
    static Ftp_Xlsx *bulid();
    void wirte(const QString &fn, const QList<QStringList> &data);

private:
    void xlsx_writeSheet(const QString &fn, const QList<QStringList> &data);

private:
    QString mFn;
    QString mDir="/tmp/xlsx/";
};

#endif // FTP_XLSX_H
