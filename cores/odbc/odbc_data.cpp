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
                  "`type` TINYINT(3) UNSIGNED NOT NULL , "
                  "`topic` TINYINT(3) UNSIGNED NOT NULL , "
                  "`indexes` TINYINT(3) UNSIGNED NOT NULL , "
                  "`value` DECIMAL(9,3) UNSIGNED NOT NULL , "
                  "`update_time` TIMESTAMP on update CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ,"
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb";
    return sqlQuery(sql.arg(cfg.db));
}

bool Odbc_Data::data_insert(const sOdbcDataIt &it)
{
    QString cmd = "INSERT INTO `pdu_data` "
                  "(`id`, `pdu_id`, `type`, `topic`, `indexes`, `value`, `update_time`) "
                  "VALUES (NULL, :pdu_id, :type, :topic, :indexes, :value, CURRENT_TIMESTAMP)";
    return data_modifyItem(it,cmd);
}

bool Odbc_Data::data_modifyItem(const sOdbcDataIt &it, const QString &cmd)
{
    uint pdu_id = getPduId(it.addr);
    QSqlQuery query(mDb); query.prepare(cmd);
    query.bindValue(":pdu_id",pdu_id);
    query.bindValue(":type",it.type);
    query.bindValue(":topic",it.topic);
    query.bindValue(":value",it.value);
    query.bindValue(":indexes",it.indexes);

    bool ret = query.exec();
    if(!ret) throwError("pdu_data", query.lastError());
    return ret;
}

bool Odbc_Data::data_update(const sOdbcDataIt &it)
{
    QString fmd = "update pdu_data set value=:value "
                  "where pdu_id=:pdu_id and type=:type "
                  "and topic=:topic and indexes=:indexes";
    return data_modifyItem(it, fmd);;
}

int Odbc_Data::data_counts(const sOdbcDataIt &it)
{
    uint pdu_id = getPduId(it.addr);
    QString fmd = "select 1 from pdu_data "
                  "where pdu_id=%1 and type=%2 "
                  "and topic=%3 and indexes=%4 "
                  "limit 1";
    QString cmd = fmd.arg(pdu_id).arg(it.type)
            .arg(it.topic).arg(it.indexes);
    return cntBySql(cmd);
}

bool Odbc_Data::data_duplicate(const sOdbcDataIt &it)
{
    QString fmd =  "INSERT INTO `pdu_data` "
                   "(`id`, `pdu_id`, `type`, `topic`, `indexes`, `value`, `update_time`) "
                   "VALUES (NULL, :pdu_id, :type, :topic, :indexes, :value, CURRENT_TIMESTAMP) "
                   "ON DUPLICATE KEY UPDATE "
                   "pdu_id=:pdu_id, type=:type, topic=:topic,indexes=:indexes";
    return data_modifyItem(it,fmd);
}

bool Odbc_Data::data_poll(const sOdbcDataIt &it)
{
    bool ret = true; //data_duplicate(it);
    if(cfg.okCnt > 1) {
        ret = data_update(it);
    } else {
        ret = data_counts(it);
        if(ret) ret = data_update(it);
        else ret = data_insert(it);
    }
    return ret;
}

