/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_alarm.h"

Odbc_Alarm::Odbc_Alarm()
{

}

bool Odbc_Alarm::alarm_createTable()
{
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_alarm` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL , "
                  "`alarm_status` VARCHAR(256) NOT NULL , "
                  "`alarm_content` VARCHAR(256) NOT NULL , "
                  "`createtime` DATETIME on update NOW() NOT NULL DEFAULT NOW() ,"
                  " FOREIGN KEY(`pdu_id`) REFERENCES `%1`.`pdu_index`(`id`) ON DELETE CASCADE ON UPDATE CASCADE , "
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb;";
    return sqlQuery(sql.arg(cfg.db));
}

bool Odbc_Alarm::alarm_insert(const sOdbcAlarmIt &it)
{
    QString cmd = "INSERT INTO `pdu_alarm` "
                  "(`id`, `pdu_id`, `alarm_status`, `alarm_content`, `createtime`) "
                  "VALUES (NULL, '%1', '%2', '%3', NOW())";
    return alarm_modifyItem(it,cmd);
}


bool Odbc_Alarm::alarm_modifyItem(const sOdbcAlarmIt &it, const QString &fmd)
{
    uint pdu_id = getPduId(it.addr); QSqlQuery query(mDb);
    QString cmd = fmd.arg(pdu_id).arg(it.alarm_status, it.alarm_content);
    query.prepare(cmd); bool ret = query.exec();
    if(!ret) throwError("pdu_alarm", query.lastError());
    return ret;
}
