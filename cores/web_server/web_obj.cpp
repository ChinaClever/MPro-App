/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_obj.h"
#include "integr_core.h"

Web_Obj::Web_Obj()
{

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
    if(ret) num = res; else qDebug() << "Error: JsonRpc Get Number" << ret;
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

int Web_Obj::getValue(uint addr, uint type, uint topic, uint sub, uint id)
{
    sDataItem it; it.addr = addr; it.type = type; if(id) id--;
    it.topic = topic; it.subtopic = sub; it.id = id;
    bool ret = Set_Core::bulid()->upMetaData(it);
    int res = -1; if(ret) res = it.value;
    return res;
}

bool Web_Obj::setting(uint addr, uint type, uint topic, uint sub, uint id, uint value)
{
    sDataItem it;
    it.addr = (0xFF & addr); it.type = type;
    it.topic = topic; it.subtopic = sub; it.id = id;
    it.rw = 1; it.value = value; it.txType = DTxType::TxWeb;
    return Set_Core::bulid()->setting(it);
}

bool Web_Obj::setCfg(uint type, uint fc, const QVariant &value, uint addr, uint sub)
{
    sCfgItem it; it.txType = DTxType::TxWeb; it.sub = sub;
    it.addr = (0xFF & addr); it.fc = fc; it.type = type;
    return Set_Core::bulid()->setCfg(it, value);
}

QString Web_Obj::getCfg(uint type, uint fc, uint addr, uint sub)
{
    sCfgItem it; it.addr = addr; it.type = type; it.fc = fc; it.sub = sub;
    return Set_Core::bulid()->getCfg(it).toString();
}

QVariant Web_Obj::log_fun(uint type, uint fc, uint id, uint cnt)
{
     sLogFcIt it;
     it.type = type; it.fc = fc;
     it.cnt = cnt; it.id = id;
     return Log_Core::bulid()->log_readFun(it);
}

QString Web_Obj::metaData(uint addr)
{
    return Integr_JsonBuild::bulid()->getJson(addr);
}
