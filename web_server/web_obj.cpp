/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_obj.h"
#include "web_core.h"

Web_Obj::Web_Obj(QObject *parent)
    : QObject{parent}
{
    mRpc = new JsonRpc_Client(this);
    bool ret = mRpc->startLocalClient(9225);
    if(!ret) qDebug() << "web rpc start local client" << ret;
}

Web_Obj *Web_Obj::bulid(QObject *parent)
{
    static Web_Obj* sington = nullptr;
    if(sington == nullptr) sington = new Web_Obj(parent);
    return sington;
}

QString Web_Obj::getString(mg_str &r, int id)
{
    QString res;
    char buffer[10] = {0};
    sprintf(buffer, "$[%d]", id);
    char *ptr = mg_json_get_str(r, buffer);
    if(ptr) {res = ptr; free(ptr);}
    return res;
}

uint Web_Obj::getNumber(mg_str &r, int id)
{
    uint num =0; double res=0; char buffer[10] = {0};  sprintf(buffer , "$[%d]" , id);
    bool ret = mg_json_get_num(r, buffer, &res);
    if(ret) num = res; else qDebug() << "Error: JsonRpc Get Number" << ret << r.ptr << id;
    return num;
}

QVector<uint> Web_Obj::getNumbers(mg_str &r, int num)
{
    QVector<uint> res;
    for(int i=0; i<num; ++i) {
        uint ret = getNumber(r, i);
        res.append(ret);
    }
    return res;
}

double Web_Obj::getValue(uint addr, uint type, uint topic, uint sub, uint id)
{
    double res = mRpc->pduDataGet(addr, type, topic, sub, id);
    return res;
}

int Web_Obj::setting(uint addr, uint type, uint topic, uint sub, uint id, double value)
{
    return mRpc->pduDataSet(addr, type, topic, sub, id, value);
}

int Web_Obj::setCfg(uint type, uint fc, const QVariant &value, uint id, uint addr)
{
    return mRpc->pduCfgSet(type, fc, value, id, addr);
}

QString Web_Obj::getCfg(uint type, uint fc, uint id, uint addr)
{
    return mRpc->pduCfgGet(type, fc, id, addr).toString();
}

QVariant Web_Obj::log_fun(uint type, uint fc, uint id, uint cnt)
{
    return mRpc->pduLogFun(type, fc, id, cnt);
}

QString Web_Obj::log_hda(const QString &start, const QString &end, int addr, int type, int topic, int index)
{
    return mRpc->pduLogHda(start, end, addr, type, topic, index);
}

QString Web_Obj::metaData(uint addr)
{
    return mRpc->pduMetaData(addr, 3);
}

QString Web_Obj::execute(const QString &cmd)
{
    return mRpc->execute(cmd);
}

QString Web_Obj::backup(int fc)
{
    return mRpc->pduCfgGet(95, fc).toString();
}

bool Web_Obj::restores(int fc, const QString &fn)
{
    return mRpc->pduCfgSet(95, fc, fn);
}

bool Web_Obj::app_upgrade(const QString &fn)
{
    return Web_Core::bulid()->app_upgrade(fn);
}

QString Web_Obj::diag()
{
    return mRpc->pduCfgGet(94, 1).toString();
}

QString Web_Obj::getIpv6En()
{
    return mRpc->pduCfgGet(41, 0 , 1).toString();
}
