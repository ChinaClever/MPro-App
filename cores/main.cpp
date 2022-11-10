/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "app_core.h"
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QObject *p = a.parent();
    App_Core::bulid(p);

    qDebug() << "Available drivers:";
    QStringList drivers = QSqlDatabase::drivers();
    foreach(QString driver, drivers)
        qDebug() << " " << driver;

//    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
//    qDebug() << "ODBC driver valid?" << db.isValid();


    return a.exec();
}
