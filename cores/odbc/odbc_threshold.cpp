/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_threshold.h"

Odbc_Threshold::Odbc_Threshold()
{

}


bool Odbc_Threshold::th_createTable()
{
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_threshold` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL , "
                  "`addr` TINYINT(3) UNSIGNED NOT NULL , "
                  "`type` TINYINT(3) UNSIGNED NOT NULL , "
                  "`topic` TINYINT(3) UNSIGNED NOT NULL , "
                  "`subtopic` TINYINT(3) UNSIGNED NOT NULL , "
                  "`indexes` TINYINT(3) UNSIGNED NOT NULL , "
                  "`value` DECIMAL(6,2) UNSIGNED NOT NULL , "
                  "`update_time` TIMESTAMP on update CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ,"
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb";
    return sqlQuery(sql.arg(cfg.db));
}


bool Odbc_Threshold::th_insert(const sOdbcThIt &it)
{
    QString cmd = "INSERT INTO `pdu_threshold` "
                  "(`id`, `pdu_id`, `addr`, `type`, `topic`, `subtopic`, `indexes`, `value`, `update_time`) "
                  "VALUES (NULL, :pdu_id, :addr, :type, :topic, :subtopic, :indexes, :value, CURRENT_TIMESTAMP)";
    return th_modifyItem(it,cmd);
}

bool Odbc_Threshold::th_modifyItem(const sOdbcThIt &it, const QString &cmd)
{
    uint pdu_id = devKey(it.addr);
    QSqlQuery query(mDb); query.prepare(cmd);
    query.bindValue(":pdu_id",pdu_id);
    query.bindValue(":type",it.type);
    query.bindValue(":topic",it.topic);
    query.bindValue(":value",it.value);
    query.bindValue(":subtopic",it.subtopic);
    if(it.addr < 255) query.bindValue(":addr",it.addr);
    if(it.indexes) query.bindValue(":indexes",it.indexes-1);

    bool ret = query.exec();
    if(!ret) {
        throwError("pdu_threshold",query.lastError());
        qDebug() << it.type <<it.topic << it.subtopic << it.indexes-1 << it.value;
    }
    return ret;
}

bool Odbc_Threshold::th_update(const sOdbcThIt &it)
{
    QString fmd = "update pdu_threshold set value=:value "
                  "where pdu_id=:pdu_id and type=:type "
                  "and topic=:topic and subtopic=:subtopic ";
    if(it.addr < 255) fmd += "and addr=:addr ";
    if(it.indexes) fmd += "and indexes=:indexes ";
    return th_modifyItem(it, fmd);;
}

int Odbc_Threshold::th_counts(const sOdbcThIt &it)
{
    uint pdu_id = devKey(it.addr);
    QString fmd = "select 1 from pdu_threshold "
                  "where pdu_id=%1 and addr=%2 and type=%3 "
                  "and topic=%4 and subtopic=%5 and indexes=%6 limit 1";
    QString cmd = fmd.arg(pdu_id).arg(it.addr).arg(it.type)
            .arg(it.topic).arg(it.subtopic).arg(it.indexes-1);
    return cntBySql(cmd);
}

bool Odbc_Threshold::th_poll(const sOdbcThIt &it)
{
    bool ret = th_counts(it);
    if(ret) ret = th_update(it);
    else if(it.addr < 255) ret = th_insert(it);
    return ret;
}

