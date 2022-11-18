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
    QString sql = "CREATE TABLE IF NOT EXISTS `pdu`.`pdu_alarm` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL , "
                  "`state` VARCHAR(256) NOT NULL , "
                  "`content` VARCHAR(256) NOT NULL , "
                  "`create_time` TIMESTAMP on update CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ,"
                  " PRIMARY KEY (`id`)) ENGINE = MyISAM;";
    return sqlQuery(sql);
}
