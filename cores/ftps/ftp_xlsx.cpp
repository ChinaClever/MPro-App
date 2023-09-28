/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ftp_xlsx.h"
#include "xlsxdocument.h"
//QXLSX_USE_NAMESPACE


Ftp_Xlsx::Ftp_Xlsx()
{

}

Ftp_Xlsx *Ftp_Xlsx::bulid()
{
    static Ftp_Xlsx* sington = nullptr;
    if(sington == nullptr) {
        sington = new Ftp_Xlsx();
    }
    return sington;
}

void Ftp_Xlsx::wirte(const QString &fn, const QList<QStringList> &data)
{
    xlsx_writeSheet(fn, data);
}


void Ftp_Xlsx::xlsx_writeSheet(const QString &fn, const QList<QStringList> &data)
{
    QXlsx::Document xlsx; xlsx.addSheet();
    for (int i = 0; i < data.size(); ++i) {
        for(int k = 0; k < data.at(i).size(); ++k)
            xlsx.write(i+1, k+1, data.at(i).at(k));
    } xlsx.saveAs(mDir + fn);
}

