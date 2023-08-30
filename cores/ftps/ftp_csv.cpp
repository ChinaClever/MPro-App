/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ftp_csv.h"
//#include "qtcsv/reader.h"
#include "qtcsv/writer.h"


Ftp_Csv::Ftp_Csv()
{

}

void Ftp_Csv::csv_saves()
{    
    for(int i=0; i<DEV_NUM; ++i) {
        csv_outletSave(i);
        csv_lineSave(i);
        csv_loopSave(i);
        csv_dualSave(i);
        csv_totalSave(i);
    }

    int t = 35 + QRandomGenerator::global()->bounded(25);
    cm::mdelay(t*1000); system("rm -rf /tmp/csv/*");
}


QString Ftp_Csv::csv_headerCommon(int addr)
{
    sDevData *dev = cm::devData(addr);
    QString room = dev->cfg.uut.room;
    QString location = dev->cfg.uut.location;
    QString ip = cm::dataPacket()->net.inet.ip;
    return QStringLiteral("%1 %2 %3 %4 ").arg(room, location, ip).arg(addr);
}

QString Ftp_Csv::csv_fileName(int addr, const QString &suffix)
{
    QString str = csv_headerCommon(addr).trimmed();
    QString file = str.replace(".", "_").replace(" ", "_");
    QDateTime dateTime = QDateTime::currentDateTime();
    str = dateTime.toString("_yyyyMMdd_hhmmss_");
    file += str + suffix + ".csv";
    return file;
}


QStringList Ftp_Csv::csv_outletHeader(int addr, QString& title)
{
    int devSpec = cm::devData(addr)->cfg.param.devSpec;
    QString str = QStringLiteral("输出位数据");
    if(cm::language()) str = "output bit data";
    title = csv_headerCommon(addr) + str;

    QStringList header; if(cm::cn()) {
        header << "时间" << "输出位";
        if(devSpec > 2) header << "开关";
        if(devSpec !=3 ) header << "电流(A)" << "有功功率(KW)" << "视在功率(KVA)" << "功率因素" << "电能(KWh)";
    } else {
        header << "time" << "output";
        if(devSpec > 2) header << "relay switch";
        if(devSpec !=3 ) header << "current(A)" << "active power(KW)" << "active power(KVA)" << "active power" << "active power(KWh)";
    }

    return header;
}

void Ftp_Csv::csv_outletSave(int addr)
{
    QString title; QString fn = csv_fileName(addr, "outlet");
    QStringList header = csv_outletHeader(addr, title);
    csv_save(fn, title, header, mCsvData[addr].outlet);
}


QStringList Ftp_Csv::csv_lineHeader(int addr, QString& title)
{
    QString str = QStringLiteral("相位数据");
    if(cm::language()) str = "line data";
    title = csv_headerCommon(addr) + str;

    QStringList header; if(cm::cn()) {
        header << "时间" << "相" << "电压(V)" << "电流(A)" << "有功功率(KW)" << "视在功率(KVA)" << "功率因素" << "电能(KWh)";
    } else {
        header << "time" << "L" << "voltage(V)" << "current(A)" << "active power(KW)" << "active power(KVA)" << "active power" << "active power(KWh)";
    }

    return header;
}

void Ftp_Csv::csv_lineSave(int addr)
{
    QString title; QString fn = csv_fileName(addr, "line");
    QStringList header = csv_lineHeader(addr, title);
    csv_save(fn, title, header, mCsvData[addr].line);
}


QStringList Ftp_Csv::csv_loopHeader(int addr, QString& title)
{
    int isBreaker = cm::devData(addr)->cfg.param.isBreaker;
    QString str = QStringLiteral("回路数据");
    if(cm::language()) str = "loop data";
    title = csv_headerCommon(addr) + str;

    QStringList header; if(cm::cn()) {
        header << "时间" << "回路";
        if(isBreaker) header << "断路器";
        header << "电压(V)" << "电流(A)" << "有功功率(KW)" << "视在功率(KVA)" << "功率因素" << "电能(KWh)";
    } else {
        header << "time" << "loop";
        if(isBreaker) header << "breaker switch";
        header << "voltage(V)" << "current(A)" << "active power(KW)" << "active power(KVA)" << "active power" << "active power(KWh)";
    }

    return header;
}

void Ftp_Csv::csv_loopSave(int addr)
{
    QString title; QString fn = csv_fileName(addr, "loop");
    QStringList header = csv_loopHeader(addr, title);
    csv_save(fn, title, header, mCsvData[addr].loop);
}

QStringList Ftp_Csv::csv_dualHeader(int addr, QString& title)
{
    QString str = QStringLiteral("服务器数据");
    if(cm::language()) str = "rack server data";
    title = csv_headerCommon(addr) + str;

    QStringList header; if(cm::cn()) {
        header << "时间" << "服务器" << "有功功率(KW)" << "视在功率(KVA)" << "功率因素" << "电能(KWh)";
    } else {
        header << "time" << "server" << "active power(KW)" << "active power(KVA)" << "active power" << "active power(KWh)";
    }

    return header;
}

void Ftp_Csv::csv_dualSave(int addr)
{
    QString title; QString fn = csv_fileName(addr, "dual");
    QStringList header = csv_dualHeader(addr, title);
    csv_save(fn, title, header, mCsvData[addr].dualPower);
}


QStringList Ftp_Csv::csv_totalHeader(int addr, QString& title)
{
    QString str = QStringLiteral("PDU统计数据");
    if(cm::language()) str = "PDU total data";
    title = csv_headerCommon(addr) + str;

    QStringList header; if(cm::cn()) {
        header << "时间" << "有功功率(KW)" << "视在功率(KVA)" << "功率因素" << "总电能(KWh)";
    } else {
        header << "time" << "active power(KW)" << "active power(KVA)" << "active power" << "active power(KWh)";
    }

    return header;
}

void Ftp_Csv::csv_totalSave(int addr)
{
    QString title; QString fn = csv_fileName(addr, "total");
    QStringList header = csv_totalHeader(addr, title);
    csv_save(fn, title, header, mCsvData[addr].total);
}

void Ftp_Csv::csv_save(const QString &fn, const QString& title, const QStringList &header, QtCSV::StringData &data)
{
    if(data.rowCount() > 0) {
        data.insertRow(0, title);
        data.insertRow(1, header);
        QString filePath = mDir + fn;
        QtCSV::Writer::write(filePath, data);
        uploads(fn);
    } data.clear();
}

void Ftp_Csv::csv_test()
{
    QtConcurrent::run(this,&Ftp_Csv::csv_testRun);
}

void Ftp_Csv::csv_testRun()
{
    QStringList strList;
    strList << "one" << "two" << "three";

    QtCSV::StringData strData;
    strData.addRow(strList);
    strData.addEmptyRow();
    strData << strList << "this is the last row";

    // write to file
    QString fn = "test.csv";
    QtCSV::Writer::write(mDir+fn, strData);
    uploads(fn);
}
