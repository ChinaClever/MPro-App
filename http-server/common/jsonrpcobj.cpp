#include "jsonrpcobj.h"

bool JsonRpcObj::getString(jsonrpc_request* r, int id, char *s)
{


}

double JsonRpcObj::getNumber(jsonrpc_request* r, int id)
{
    double res=0; char buffer[10] = {0};  sprintf(buffer , "$[%d]" , id);
    bool ret = mjson_get_number(r->params, r->params_len, buffer, &res);
    if(!ret) qDebug() << "Error: JsonRpcObj Get Number" << ret;
    return res;
}

QVector<double> JsonRpcObj::getNumbers(jsonrpc_request* r, int num)
{
    QVector<double> res;
    for(int i=0; i<num; ++i) {
        double ret = getNumber(r, i);
        res.append(ret);
    }
    return res;
}
