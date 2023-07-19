/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_uut.h"

Odbc_Uut::Odbc_Uut()
{

}


bool Odbc_Uut::uut_createTable()
{
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_uut` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL , "
                  "`room_name` VARCHAR(64) NULL ,"
                  "`dev_position` VARCHAR(64) NULL ,"

                  "`dev_type` VARCHAR(64) NULL ,"
                  "`dev_name` VARCHAR(64) NULL ,"
                  "`dev_sn` VARCHAR(64) NULL ,"
                  "`qrcode` VARCHAR(128) NULL ,"
                  "`createtime` DATETIME NOT NULL DEFAULT NOW(),"
                  "`updatetime` DATETIME on update NOW() NOT NULL DEFAULT NOW() ,"
                  " FOREIGN KEY(`pdu_id`) REFERENCES `%1`.`pdu_index`(`id`) ON DELETE CASCADE ON UPDATE CASCADE , "
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb";
    return sqlQuery(sql.arg(cfg.db));
}


bool Odbc_Uut::uut_insert(const sOdbcUutIt &it)
{
    QString cmd = "INSERT INTO `pdu_uut` "
                  "(`id`, `pdu_id`, `room_name`, `dev_position`, "
                  "`dev_type`, `dev_name`, `dev_sn`, `qrcode`, `createtime`) "
                  "VALUES (NULL, :pdu_id, :room_name, :dev_position, "
                  ":dev_type, :dev_name, :dev_sn, :qrcode, NOW())";
    return uut_modifyItem(it,cmd);
}

bool Odbc_Uut::uut_modifyItem(const sOdbcUutIt &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);

    query.bindValue(":pdu_id",item.pdu_id);
    query.bindValue(":room_name",item.room_name);
    query.bindValue(":dev_position",item.dev_position);
    query.bindValue(":dev_type",item.dev_type);
    query.bindValue(":dev_name",item.dev_name);
    query.bindValue(":dev_sn",item.dev_sn);
    query.bindValue(":qrcode",item.qrcode);

    bool ret = query.exec();
    if(!ret) throwError("pdu_dev", query.lastError());
    return ret;
}

bool Odbc_Uut::uut_update(const sOdbcUutIt &it)
{
    QString fmd = "update `pdu_uut` set "
                  "room_name        = :room_name,"
                  "dev_position     = :dev_position,"
                  "dev_type         = :dev_type,"
                  "dev_name         = :dev_name,"
                  "dev_sn           = :dev_sn,"
                  "qrcode           = :qrcode"
                  " where pdu_id   = :pdu_id ";
    return uut_modifyItem(it, fmd);
}

int Odbc_Uut::uut_counts(const sOdbcUutIt &it)
{
    QString fmd = "select 1 from pdu_uut where pdu_id='%1'  limit 1";
    QString cmd = fmd.arg(it.pdu_id);
    return cntBySql(cmd);
}


bool Odbc_Uut::uut_polls()
{
    int ret = false;
    if(cfg.okCnt < 5) mDb.transaction();
    int num = cm::masterDev()->cfg.nums.slaveNum;
    for(int i=0; i<=num; ++i) ret = uut_poll(i);
    if(cfg.okCnt < 5) mDb.commit();
    return ret;
}

bool Odbc_Uut::uut_poll(int addr)
{
    sOdbcUutIt it;
    uut_params(addr, it);
    bool ret = uut_counts(it);
    if(ret) ret = uut_update(it);
    else ret = uut_insert(it);
    return ret;
}

bool Odbc_Uut::uut_params(int addr, sOdbcUutIt &it)
{
    sDevData *dev = cm::devData(addr);
    it.pdu_id = getPduId(addr);

    sDevCfg *devCfg = &(dev->cfg);
    sUutInfo *uut = &(devCfg->uut);
    it.dev_type = uut->devType;
    it.room_name = uut->room;
    it.dev_position = uut->location;
    it.dev_name = uut->devName;
    it.qrcode = uut->qrcode;
    it.dev_sn = uut->sn;

    return it.pdu_id;
}
