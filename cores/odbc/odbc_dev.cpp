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
                  "`dev_key` VARCHAR(64) NOT NULL , "
                  "`cascade_addr` TINYINT(3) UNSIGNED NOT NULL , "
                  "`run_status` TINYINT(2) UNSIGNED NOT NULL , "
                  "`dev_type` VARCHAR(64) NULL ,"
                  "`room_name` VARCHAR(64) NULL ,"
                  "`dev_position` VARCHAR(64) NULL ,"
                  "`sw_version` VARCHAR(64) NULL ,"
                  "`ver_md` VARCHAR(32) NULL ,"

                  "`dev_mode` TINYINT(3) UNSIGNED NOT NULL , "
                  "`slave_num` TINYINT(3) UNSIGNED NOT NULL , "
                  "`line_num` TINYINT(3) UNSIGNED NOT NULL , "
                  "`loop_num` TINYINT(3) UNSIGNED NOT NULL , "
                  "`output_num` TINYINT(3) UNSIGNED NOT NULL , "
                  "`dev_spec` TINYINT(3) UNSIGNED NOT NULL , "
                  "`rtu_addr` TINYINT(3) UNSIGNED NOT NULL , "

                  "`mac` VARCHAR(32) NULL ,"
                  "`ip_v4` VARCHAR(32) NULL ,"
                  "`ip_v6` VARCHAR(32) NULL ,"
                  "`qrcode` VARCHAR(128) NULL ,"
                  "`create_time` TIMESTAMP NOT NULL ,"
                  "`update_time` TIMESTAMP on update CURRENT_TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ,"
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb";
    return sqlQuery(sql.arg(cfg.db));
}

bool Odbc_Dev::dev_insert(const sOdbcDevIt &it)
{
    QString cmd = "INSERT INTO `pdu_dev` "
                  "(`id`, `dev_key`, `cascade_addr`, `run_status`, `dev_type`, "
                  "`room_name`, `dev_position`, `sw_version`, `ver_md`, "
                  "`dev_mode`, `slave_num`, `line_num`, `loop_num`, "
                  "`output_num`, `dev_spec`, `rtu_addr`, `mac`, "
                  "`ip_v4`, `ip_v6`, `qrcode`, `create_time`) "

                  "VALUES (NULL, :dev_key, :cascade_addr, :run_status, :dev_type, "
                  ":room_name, :dev_position, :sw_version, :ver_md,"
                  ":dev_mode, :slave_num, :line_num, :loop_num,"
                  ":output_num, :dev_spec, :rtu_addr, :mac,"
                  ":ip_v4, :ip_v6, :qrcode, CURRENT_TIMESTAMP)";
    return dev_modifyItem(it,cmd);
}

bool Odbc_Dev::dev_modifyItem(const sOdbcDevIt &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);

    query.bindValue(":dev_key",item.dev_key);
    query.bindValue(":cascade_addr",item.cascade_addr);
    query.bindValue(":run_status",item.run_status);
    query.bindValue(":dev_type",item.dev_type);
    query.bindValue(":room_name",item.room_name);
    query.bindValue(":dev_position",item.dev_position);
    query.bindValue(":sw_version",item.sw_version);
    query.bindValue(":ver_md",item.ver_md);
    query.bindValue(":dev_mode",item.dev_mode);

    query.bindValue(":slave_num",item.slave_num);
    query.bindValue(":line_num",item.line_num);
    query.bindValue(":loop_num",item.loop_num);
    query.bindValue(":output_num",item.output_num);
    query.bindValue(":dev_spec",item.dev_spec);
    query.bindValue(":rtu_addr",item.rtu_addr);
    query.bindValue(":mac",item.mac);
    query.bindValue(":ip_v4",item.ip_v4);
    query.bindValue(":ip_v6",item.ip_v6);
    query.bindValue(":qrcode",item.qrcode);

    bool ret = query.exec();
    if(!ret) throwError(query.lastError());
    return ret;
}

bool Odbc_Dev::dev_update(const sOdbcDevIt &it)
{
    QString fmd = "update `pdu_dev` set "
                  "run_status       = :run_status,"
                  "dev_type         = :dev_type,"
                  "room_name        = :room_name,"
                  "dev_position     = :dev_position,"
                  "ver_md           = :ver_md,"
                  "dev_mode         = :dev_mode,"
                  "slave_num        = :slave_num,"
                  "line_num         = :line_num,"
                  "loop_num         = :loop_num,"
                  "output_num       = :output_num,"
                  "dev_spec         = :dev_spec,"
                  "rtu_addr         = :rtu_addr,"
                  "mac              = :mac,"
                  "ip_v4            = :ip_v4,"
                  "ip_v6            = :ip_v6,"
                  "qrcode           = :qrcode"
                  " where dev_key   = :dev_key "
                  " and cascade_addr = :cascade_addr";
    return dev_modifyItem(it, fmd);
}

int Odbc_Dev::dev_counts(const sOdbcDevIt &it)
{
    QString fmd = "select 1 from pdu_dev where dev_key='%1' and cascade_addr=%2 limit 1";
    QString cmd = fmd.arg(it.dev_key).arg(it.cascade_addr);
    return cntBySql(cmd);
}

bool Odbc_Dev::dev_polls()
{
    int ret = false;
    if(cfg.pdukey.size()) {
        mKeys.clear(); if(cfg.okCnt==1) mDb.transaction();
        int num = cm::masterDev()->cfg.nums.slaveNum;
        for(int i=0; i<=num; ++i) ret = dev_poll(i);
        if(cfg.okCnt==1) {mDb.commit(); cm::mdelay(100); mDb.transaction();}
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

uint Odbc_Dev::devKey(int addr)
{
    if(!mKeys.contains(addr)) {
        QString fmd = "SELECT `id` FROM `pdu_dev` where dev_key='%1' and cascade_addr=%2";
        QString cmd = fmd.arg(cfg.pdukey).arg(addr);
        uint id = cntBySql(cmd);
        if(id) mKeys[addr] = id;
    }
    return mKeys[addr];
}

void Odbc_Dev::dev_params(int addr, sOdbcDevIt &it)
{
    sDevData *dev = cm::devData(addr);
    it.cascade_addr = addr;
    it.dev_key = cfg.pdukey;
    it.run_status = dev->status;

    sDevCfg *devCfg = &(dev->cfg);
    sUutInfo *uut = &(devCfg->uut);
    it.dev_type = uut->devType;
    it.room_name = uut->room;
    it.dev_position = uut->location;
    it.qrcode = uut->qrcode;

    it.sw_version = devCfg->vers.ver;
    it.ver_md = devCfg->vers.md5;

    sParameter *param = &devCfg->param;
    it.dev_mode = param->devMode;
    it.dev_spec = param->devSpec;
    it.rtu_addr = param->modbusRtuAddr;

    sDevNums *nums = &devCfg->nums;
    it.slave_num = nums->slaveNum;
    it.line_num = nums->lineNum;
    it.loop_num = nums->loopNum;
    it.output_num = nums->outputNum;

    if(0 == addr) {
        sNetInterface *net = &(cm::dataPacket()->net);
        it.mac = net->mac;
        it.ip_v4 = net->inet.ip;
        it.ip_v6 = net->inet6.ip;
    }
}
