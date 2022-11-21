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
                  "`addr` TINYINT(3) UNSIGNED NOT NULL , "
                  "`type` TINYINT(3) UNSIGNED NOT NULL , "
                  "`topic` TINYINT(3) UNSIGNED NOT NULL , "
                  "`subindex` TINYINT(3) UNSIGNED NOT NULL , "
                  "`value` DECIMAL(7,2) UNSIGNED NOT NULL , "
                  "`create_time` TIMESTAMP on update CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ,"
                  " PRIMARY KEY (`id`)) ENGINE = MyISAM";
    return sqlQuery(sql.arg(cfg.db));
}


bool Odbc_Hda::hda_insert(const sOdbcHdaIt &it)
{
    QString cmd = "INSERT INTO `pdu_hda` "
                  "(`id`, `pdu_id`, `addr`, `type`, `topic`, `subindex`, `value`, `create_time`) "
                  "VALUES (NULL, '%1', '%2', '%3', '%4', '%5', '%6', CURRENT_TIMESTAMP)";
   return hda_modifyItem(it,cmd);
}


bool Odbc_Hda::hda_modifyItem(const sOdbcHdaIt &it, const QString &fmd)
{
    QSqlQuery query(mDb);
    QString cmd = fmd.arg(m_pdu_id).arg(it.addr).arg(it.type)
            .arg(it.topic).arg(it.subindex).arg(it.value);
    query.prepare(cmd);
    bool ret = query.exec();
    if(!ret) throwError(query.lastError());
    return ret;
}
