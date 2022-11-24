/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_event.h"

Odbc_Event::Odbc_Event()
{

}


bool Odbc_Event::event_createTable()
{    
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_event` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL , "
                  "`event_type` VARCHAR(256) NOT NULL , "
                  "`event_content` VARCHAR(256) NOT NULL , "
                  "`create_time` TIMESTAMP on update CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ,"
                  " PRIMARY KEY (`id`)) ENGINE = MyISAM;";
    return sqlQuery(sql.arg(cfg.db));
}

bool Odbc_Event::event_insert(const sOdbcEventIt &it)
{
    QString cmd = "INSERT INTO `pdu_event` "
                  "(`id`, `pdu_id`, `event_type`, `event_content`, `create_time`) "
                  "VALUES (NULL, '%1', '%2', '%3', CURRENT_TIMESTAMP)";
    return event_modifyItem(it, cmd);
}


bool Odbc_Event::event_modifyItem(const sOdbcEventIt &it, const QString &fmd)
{
    uint pdu_id = devKey(it.addr); QSqlQuery query(mDb);
    QString cmd = fmd.arg(pdu_id) .arg(it.event_type, it.event_content);
    query.prepare(cmd); bool ret = query.exec();
    if(!ret) throwError("pdu_event", query.lastError());
    return ret;
}
