/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_obj.h"

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

double Web_Obj::getNumber(mg_str &r, int id)
{
    double res=0; char buffer[10] = {0};  sprintf(buffer , "$[%d]" , id);
    bool ret = mg_json_get_num(r, buffer, &res);
    if(!ret) qDebug() << "Error: JsonRpc Get Number" << ret;
    return res;
}

QVector<double> Web_Obj::getNumbers(mg_str &r, int num)
{
    QVector<double> res;
    for(int i=0; i<num; ++i) {
        double ret = getNumber(r, i);
        res.append(ret);
    }
    return res;
}

int Web_Obj::getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id)
{
    sDataItem it; it.addr = addr; it.type = type; if(id) id--;
    it.topic = topic; it.subtopic = sub; it.id = id;
    bool ret = Set_Core::bulid()->upMetaData(it);
    int res = -1; if(ret) res = it.value;
    return res;
}

bool Web_Obj::setting(uint addr, uchar type, uchar topic, uchar sub, uchar id, uint value)
{
    sDataItem it;
    it.addr = (0xFF & addr); it.type = type;
    it.topic = topic; it.subtopic = sub; it.id = id;
    it.rw = 1; it.value = value; it.txType = DTxType::TxWeb;
    return Set_Core::bulid()->setting(it);
}

bool Web_Obj::setCfg(uint addr, uchar fc, uchar type, const QVariant &value)
{
    sCfgItem it; it.txType = DTxType::TxWeb;
    it.addr = (0xFF & addr); it.fc = type; it.type = fc;
    return Set_Core::bulid()->setCfg(it, value);
}

QString Web_Obj::getCfg(uchar addr, uchar fc, uchar id)
{
    sCfgItem it; it.addr = addr; it.type = fc; it.fc = id;
    return Set_Core::bulid()->getCfg(it).toString();
}

QVariant Web_Obj::log_fun(uchar type, uchar fc, uint id, uint cnt)
{
     sLogFcIt it;
     it.type = type; it.fc = fc;
     it.cnt = cnt; it.id = id;
     return Log_Core::bulid()->log_readFun(it);
}
