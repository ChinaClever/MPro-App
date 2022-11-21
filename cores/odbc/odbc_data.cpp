/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_data.h"

Odbc_Data::Odbc_Data()
{

}


bool Odbc_Data::data_createTable()
{
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_data` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL , "
                  "`addr` TINYINT(3) UNSIGNED NOT NULL , "
                  "`type` TINYINT(3) UNSIGNED NOT NULL , "
                  "`topic` TINYINT(3) UNSIGNED NOT NULL , "
                  "`indexes` TINYINT(3) UNSIGNED NOT NULL , "
                  "`value` DECIMAL(7,2) UNSIGNED NOT NULL , "
                  "`update_time` TIMESTAMP on update CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ,"
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb";
    return sqlQuery(sql.arg(cfg.db));
}

bool Odbc_Data::data_insert(const sOdbcDataIt &it)
{
    QString cmd = "INSERT INTO `pdu_data` "
                  "(`id`, `pdu_id`, `addr`, `type`, `topic`, `indexes`, `value`, `update_time`) "
                  "VALUES (NULL, '%1', '%2', '%3', '%4', '%5', '%6', CURRENT_TIMESTAMP)";
   return data_modifyItem(it,cmd);
}

bool Odbc_Data::data_modifyItem(const sOdbcDataIt &it, const QString &fmd)
{
   uint pdu_id = devKey(it.addr); QSqlQuery query(mDb);
    QString cmd = fmd.arg(pdu_id).arg(it.addr).arg(it.type)
            .arg(it.topic).arg(it.indexes).arg(it.value);
    query.prepare(cmd); bool ret = query.exec();
    if(!ret) throwError(query.lastError());
    return ret;
}

bool Odbc_Data::data_update(const sOdbcDataIt &it)
{
    QString fmd = "update pdu_data set value=%6 "
                  "where pdu_id=%1 and addr=%2 and type=%3 "
                  "and topic=%4 and indexes=%5";
    return data_modifyItem(it, fmd);;
}

int Odbc_Data::data_counts(const sOdbcDataIt &it)
{
    uint pdu_id = devKey(it.addr);
    QString fmd = "select 1 from pdu_data "
                  "where pdu_id=%1 and addr=%2 and type=%3 "
                  "and topic=%4 and indexes=%5 "
                  "limit 1";
    QString cmd = fmd.arg(pdu_id).arg(it.addr).arg(it.type)
            .arg(it.topic).arg(it.indexes);
    return cntBySql(cmd);
}

bool Odbc_Data::data_poll(const sOdbcDataIt &it)
{
    bool ret = data_counts(it);
    if(ret) ret = data_update(it);
    else ret = data_insert(it);
    return ret;
}

