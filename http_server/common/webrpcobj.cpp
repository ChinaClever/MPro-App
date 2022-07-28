#include "webrpcobj.h"

WebRpcObj::WebRpcObj()
{
    compileTime();
}

QString WebRpcObj::getString(mg_str &r, int id)
{
    QString res;
    char buffer[10] = {0};
    sprintf(buffer, "$[%d]", id);
    char *ptr = mg_json_get_str(r, buffer);
    if(ptr) {res = ptr; free(ptr);}
    return res;
}

double WebRpcObj::getNumber(mg_str &r, int id)
{
    double res=0; char buffer[10] = {0};  sprintf(buffer , "$[%d]" , id);
    bool ret = mg_json_get_num(r, buffer, &res);
    if(!ret) qDebug() << "Error: JsonRpc Get Number" << ret;
    return res;
}

QVector<double> WebRpcObj::getNumbers(mg_str &r, int num)
{
    QVector<double> res;
    for(int i=0; i<num; ++i) {
        double ret = getNumber(r, i);
        res.append(ret);
    }
    return res;
}


void WebRpcObj::compileTime()
{
    sVersions *vers = &(cm::masterDev()->cfg.vers);
    QString str = cm::buildDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(vers->webCompileTime, str.toLatin1().data());
}
