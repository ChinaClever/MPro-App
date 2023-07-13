/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_hda.h"

Odbc_Hda::Odbc_Hda()
{

}

bool Odbc_Hda::hda_createTable()
{
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_hda` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL , "
                  "`type` TINYINT(3) UNSIGNED NOT NULL , "
                  "`topic` TINYINT(3) UNSIGNED NOT NULL , "
                  "`indexes` TINYINT(3) UNSIGNED NOT NULL , "
                  "`value` DECIMAL(9,3) UNSIGNED NOT NULL , "
                  "`create_time` TIMESTAMP on update CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ,"
                  " FOREIGN KEY(`pdu_id`) REFERENCES `%1`.`pdu_index`(`id`) ON DELETE CASCADE ON UPDATE CASCADE , "
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb";
    return sqlQuery(sql.arg(cfg.db));
}


bool Odbc_Hda::hda_insert(const sOdbcHdaIt &it)
{
    QString cmd = "INSERT INTO `pdu_hda` "
                  "(`id`, `pdu_id`, `type`, `topic`, `indexes`, `value`, `create_time`) "
                  "VALUES (NULL, '%1', '%2', '%3', '%4', '%5', CURRENT_TIMESTAMP)";
   return hda_modifyItem(it,cmd);
}


bool Odbc_Hda::hda_modifyItem(const sOdbcHdaIt &it, const QString &fmd)
{
    uint pdu_id = getPduId(it.addr); QSqlQuery query(mDb);
    QString cmd = fmd.arg(pdu_id).arg(it.type).arg(it.topic)
            .arg(it.indexes).arg(it.value);
    query.prepare(cmd); bool ret = query.exec();
    if(!ret) throwError("pdu_hda", query.lastError());
    return ret;
}
