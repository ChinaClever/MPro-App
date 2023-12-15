#ifndef FTP_CSV_H
#define FTP_CSV_H
#include "ftp_upload.h"
#include "qtcsv/stringdata.h"

class Ftp_Csv : public Ftp_Upload
{
public:
    Ftp_Csv();

    void csv_test();
    void csv_saves();
    void csv_testRun();
    void csv_appendLine(int addr, const QStringList& values) {mCsvData[addr].line.addRow(values);}
    void csv_appendLoop(int addr, const QStringList& values) {mCsvData[addr].loop.addRow(values);}
    void csv_appendTotal(int addr, const QStringList& values) {mCsvData[addr].total.addRow(values);}
    void csv_appendOutlet(int addr, const QStringList& values) {mCsvData[addr].outlet.addRow(values);}
    void csv_appendDual(int addr, const QStringList& values) {mCsvData[addr].dualPower.addRow(values);}

private:
    void csv_lineSave(int addr);
    void csv_loopSave(int addr);
    void csv_dualSave(int addr);
    void csv_outletSave(int addr);
    void csv_totalSave(int addr);

    QString csv_headerCommon(int addr);
    QString csv_fileName(int addr, const QString &suffix);
    QStringList csv_lineHeader(int addr, QString& title);
    QStringList csv_loopHeader(int addr, QString& title);
    QStringList csv_outletHeader(int addr, QString &title);
    QStringList csv_dualHeader(int addr, QString& title);
    QStringList csv_totalHeader(int addr, QString& title);
    void csv_save(const QString &fn, const QString &title,
                  const QStringList &header, QtCSV::StringData &data);

private:
    struct sCsvData {
        QtCSV::StringData total;
        QtCSV::StringData line;
        QtCSV::StringData loop;
        QtCSV::StringData outlet;
        QtCSV::StringData dualPower;
    } mCsvData[DEV_NUM];
};

#endif // FTP_CSV_H
