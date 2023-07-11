/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_dev.h"

Odbc_Dev::Odbc_Dev()
{

}

bool Odbc_Dev::dev_createTable()
{    
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_dev` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`pdu_id` INT(11) UNSIGNED NOT NULL , "
                  "`run_status` TINYINT(2) UNSIGNED NOT NULL , "

                  "`room_name` VARCHAR(64) NULL ,"
                  "`dev_position` VARCHAR(64) NULL ,"
                  "`cascade_addr` TINYINT(3) UNSIGNED NOT NULL , "

                  "`dev_type` VARCHAR(64) NULL ,"
                  "`dev_spec` TINYINT(3) UNSIGNED NOT NULL , "
                  "`dev_mode` TINYINT(3) UNSIGNED NOT NULL , "
                  "`slave_num` TINYINT(3) UNSIGNED NOT NULL , "
                  "`line_num` TINYINT(3) UNSIGNED NOT NULL , "
                  "`loop_num` TINYINT(3) UNSIGNED NOT NULL , "
                  "`output_num` TINYINT(3) UNSIGNED NOT NULL , "

                  "`sw_version` VARCHAR(64) NULL ,"
                  "`sn` VARCHAR(32) NULL ,"
                  "`qrcode` VARCHAR(128) NULL ,"
                  "`create_time` TIMESTAMP NOT NULL ,"
                  "`update_time` TIMESTAMP on update CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ,"
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb";
    return sqlQuery(sql.arg(cfg.db));
}

bool Odbc_Dev::dev_insert(const sOdbcDevIt &it)
{
    QString cmd = "INSERT INTO `pdu_dev` "
                  "(`id`, `pdu_id`, `run_status`, `room_name`, `dev_position`, `cascade_addr`, "
                  "`dev_type`, `dev_spec`, `dev_mode`, `slave_num`, `line_num`, `loop_num`, `output_num`, "
                  "`sw_version`, `sn`, `qrcode`, `create_time`) "

                  "VALUES (NULL, :pdu_id, :run_status, :room_name, :dev_position, :cascade_addr,"
                  ":dev_type, :dev_spec, :dev_mode, :slave_num, :line_num, :loop_num, :output_num,"
                  ":sw_version, :sn, :qrcode, CURRENT_TIMESTAMP)";
    return dev_modifyItem(it,cmd);
}

bool Odbc_Dev::dev_modifyItem(const sOdbcDevIt &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);

    query.bindValue(":pdu_id",item.pdu_id);
    query.bindValue(":run_status",item.run_status);
    query.bindValue(":room_name",item.room_name);
    query.bindValue(":dev_position",item.dev_position);
    query.bindValue(":cascade_addr",item.cascade_addr);

    query.bindValue(":dev_type",item.dev_type);
    query.bindValue(":dev_spec",item.dev_spec);
    query.bindValue(":dev_mode",item.dev_mode);
    query.bindValue(":slave_num",item.slave_num);
    query.bindValue(":line_num",item.line_num);
    query.bindValue(":loop_num",item.loop_num);
    query.bindValue(":output_num",item.output_num);
    query.bindValue(":sw_version",item.sw_version);

    query.bindValue(":sn",item.sn);
    query.bindValue(":qrcode",item.qrcode);

    bool ret = query.exec();
    if(!ret) throwError("pdu_dev", query.lastError());
    return ret;
}

bool Odbc_Dev::dev_update(const sOdbcDevIt &it)
{
    QString fmd = "update `pdu_dev` set "
                  "run_status       = :run_status,"
                  "room_name        = :room_name,"
                  "dev_position     = :dev_position,"
                  "cascade_addr     = :cascade_addr,"

                  "dev_type         = :dev_type,"
                  "dev_spec         = :dev_spec,"
                  "dev_mode         = :dev_mode,"
                  "slave_num        = :slave_num,"
                  "line_num         = :line_num,"
                  "loop_num         = :loop_num,"
                  "output_num       = :output_num,"


                  "sw_version       = :sw_version,"
                  "sn               = :sn,"
                  "qrcode           = :qrcode"
                  " where pdu_id   = :pdu_id ";
    return dev_modifyItem(it, fmd);
}

int Odbc_Dev::dev_counts(const sOdbcDevIt &it)
{
    QString fmd = "select 1 from pdu_dev where pdu_id='%1'  limit 1";
    QString cmd = fmd.arg(it.pdu_id);
    return cntBySql(cmd);
}

bool Odbc_Dev::dev_polls()
{
    int ret = false;
    if(cfg.pdukey.size()) {
        if(cfg.okCnt==1) mDb.transaction();
        int num = cm::masterDev()->cfg.nums.slaveNum;
        for(int i=0; i<=num; ++i) ret = dev_poll(i);
        if(cfg.okCnt==1) mDb.commit();
    }
    return ret;
}

bool Odbc_Dev::dev_poll(int addr)
{
    sOdbcDevIt it;
    dev_params(addr, it);
    bool ret = dev_counts(it);
    if(ret) ret = dev_update(it);
    else ret = dev_insert(it);
    return ret;
}

//uint Odbc_Dev::devKey(int addr)
//{
//    if(!mKeys.contains(addr)) {
//        QString fmd = "SELECT `id` FROM `pdu_dev` where pdu_id='%1' and cascade_addr=%2";
//        QString cmd = fmd.arg(cfg.pdukey).arg(addr);
//        uint id = cntBySql(cmd);
//        if(id) mKeys[addr] = id;
//    }
//    return mKeys[addr];
//}

void Odbc_Dev::dev_params(int addr, sOdbcDevIt &it)
{
    sDevData *dev = cm::devData(addr);
    it.cascade_addr = addr;
    it.pdu_id = getPduId(addr);
    it.run_status = dev->status;

    sDevCfg *devCfg = &(dev->cfg);
    sUutInfo *uut = &(devCfg->uut);
    it.dev_type = uut->devType;
    it.room_name = uut->room;
    it.dev_position = uut->location;
    it.qrcode = uut->qrcode;
    it.sn = uut->sn;

    it.sw_version = devCfg->vers.ver;
    sParameter *param = &devCfg->param;
    it.dev_mode = param->devMode;
    it.dev_spec = param->devSpec;

    sDevNums *nums = &devCfg->nums;
    it.slave_num = nums->slaveNum;
    it.line_num = nums->lineNum;
    it.loop_num = nums->loopNum;
    it.output_num = nums->outputNum;

}
