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
    qDebug() << db_open() << data_createTable();

    sOdbcDataIt it;
    it.addr = 12;
    it.type = 3;
    it.topic = 4;
    it.subindex = 5;
    it.value = 26.9;
    qDebug() << data_poll(it);
}
