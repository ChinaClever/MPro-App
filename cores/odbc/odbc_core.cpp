/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_core.h"
#include <QtConcurrent>

Odbc_Core::Odbc_Core()
{
    QtConcurrent::run(this,&Odbc_Core::run);
}

Odbc_Core *Odbc_Core::bulid()
{
    static Odbc_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Odbc_Core();
    }
    return sington;
}

void Odbc_Core::run()
{

    qDebug() << db_open() << event_createTable();
}
