/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_index.h"

Odbc_Index::Odbc_Index()
{

}

bool Odbc_Index::index_createTable()
{
    QString sql = "CREATE TABLE IF NOT EXISTS `%1`.`pdu_index` ( "
                  "`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT , "
                  "`uuid` VARCHAR(64) NULL , "
                  "`dev_key` VARCHAR(64) NULL , "
                  "`createtime` DATETIME NOT NULL DEFAULT NOW(),"
                  "`updatetime` DATETIME on update NOW() NOT NULL DEFAULT NOW(),"
                  " PRIMARY KEY (`id`)) ENGINE = InnoDb";
    return sqlQuery(sql.arg(cfg.db));
}


bool Odbc_Index::index_insert(const sOdbcIndexIt &it)
{
    QString cmd = "INSERT INTO `pdu_index` "
                  "(`id`, `uuid`, `dev_key`,`createtime`) "
                  "VALUES (NULL, :uuid, :dev_key, NOW())";
    return index_modifyItem(it,cmd);
}

bool Odbc_Index::index_modifyItem(const sOdbcIndexIt &item, const QString &cmd)
{
    QSqlQuery query(mDb); query.prepare(cmd);
    query.bindValue(":dev_key",item.dev_key);
    query.bindValue(":uuid",item.uuid);

    bool ret = query.exec();
    if(!ret) throwError("pdu_index", query.lastError());
    return ret;
}

int Odbc_Index::index_counts(const sOdbcIndexIt &it)
{
    QString fmd = "select 1 from pdu_index where uuid='%1' limit 1";
    QString cmd = fmd.arg(it.uuid);
    return cntBySql(cmd);
}

bool Odbc_Index::index_counts(const QString &uuid, const QString &key)
{
    QString fmd = "select 1 from pdu_index where uuid='%1' and dev_key='%2' limit 1";
    QString cmd = fmd.arg(uuid, key);
    return cntBySql(cmd);
}

bool Odbc_Index::index_update(const sOdbcIndexIt &it)
{
    QString fmd = "update `pdu_index` set "
                  "dev_key       = :dev_key"
                  " where uuid   = :uuid ";
    return index_modifyItem(it, fmd);
}

bool Odbc_Index::index_poll(const QString &uuid)
{
    sOdbcIndexIt it; it.uuid = uuid;
    it.dev_key = cfg.pdukey+":"+m_addr;
    bool ret = index_counts(it);
    if(ret) ret = index_update(it);
    else ret = index_insert(it);
    return ret;
}

uint Odbc_Index::getPduId(const QString &uuid)
{
    uint id = 0;
    if(mKeys.contains(uuid)) id = mKeys[uuid];
    if(0 == id) {
        id = index_uuid(uuid);
        if(0 == id) {
            mDb.transaction();
            index_poll(uuid);
            mDb.commit(); cm::mdelay(100);
            id = index_uuid(uuid);
        }
    }

    return id;
}

uint Odbc_Index::index_uuid(const QString &uuid)
{
    QString fmd = "SELECT `id` FROM `pdu_index` where uuid='%1'";
    QString cmd = fmd.arg(uuid);
    mKeys[uuid] = cntBySql(cmd);
    //cout << cmd << mKeys;
    return mKeys[uuid];
}

uint Odbc_Index::getPduId(int addr)
{
    m_addr = QString::number(addr);
    char *uuid = cm::devData(addr)->cfg.uut.uuid;
    return getPduId(uuid);
}
