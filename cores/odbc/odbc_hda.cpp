/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_hda.h"
#define ODBC_NO_SHOW 1

Odbc_Hda::Odbc_Hda()
{

}

bool Odbc_Hda::hda_createTable()
{
#if ODBC_NO_SHOW
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_hda` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL , "
                  "`type` TINYINT(3) UNSIGNED NOT NULL , "
                  "`topic` TINYINT(3) UNSIGNED NOT NULL , "
                  "`indexes` TINYINT(3) UNSIGNED NOT NULL , "
                  "`value` DECIMAL(9,3) UNSIGNED NOT NULL , "
                  "`createtime` DATETIME on update NOW() NOT NULL DEFAULT NOW() ,"
                  " FOREIGN KEY(`pdu_id`) REFERENCES `%1`.`pdu_index`(`id`) ON DELETE CASCADE ON UPDATE CASCADE , "
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb";
#else
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_hda` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL UNIQUE , ";
    //"`type` TINYINT(3) UNSIGNED NOT NULL , "
    //"`topic` TINYINT(3) UNSIGNED NOT NULL , "
    if(cm::cn()) {
        sql += "`type` ENUM('总', '相', '回路', '输出位', '组', '机架', '环境') , "
               "`topic` ENUM('电压', '电流', '功率', '电能', '功率因数', '视在功率', '温度', '湿度' ) ,";
    } else {
        sql +=  "`type` ENUM('total', 'phase', 'loop', 'outlet', 'group', 'rack', 'environment') , "
               "`topic` ENUM('switch', 'voltage', 'current', 'power', 'electricity', 'PF', 'apparent-power', "
               "'temperature', 'humidity' ) ,";
    }

    sql += "`indexes` TINYINT(3) UNSIGNED NOT NULL , "
           "`value` DECIMAL(9,3) UNSIGNED NOT NULL , "
           "`createtime` DATETIME on update NOW() NOT NULL DEFAULT NOW() ,"
           " FOREIGN KEY(`pdu_id`) REFERENCES `%1`.`pdu_index`(`id`) ON DELETE CASCADE ON UPDATE CASCADE , "
           " PRIMARY KEY (`id`)) ENGINE = InnoDb";

#endif
    return sqlQuery(sql.arg(cfg.db));
}

bool Odbc_Hda::hda_insert(const sOdbcHdaIt &it)
{
#if ODBC_NO_SHOW
    sOdbcHdaIt item = it;
#else
    if(DTopic::Relay == it.topic) return false;
    if(it.type > 6 || it.topic > 12) return false;
    if(it.topic > 7 && it.topic < 11) return false;

    sOdbcHdaIt item = it;
    if(item.topic > 10) item.topic -= 3;
    item.type += 1; item.topic -= 1;
#endif
    QString cmd = "INSERT INTO `pdu_hda` "
                  "(`id`, `pdu_id`, `type`, `topic`, `indexes`, `value`, `createtime`) "
                  "VALUES (NULL, '%1', '%2', '%3', '%4', '%5', NOW())";
    return hda_modifyItem(item,cmd);
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
