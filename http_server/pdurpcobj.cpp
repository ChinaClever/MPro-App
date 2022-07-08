#include "pdurpcobj.h"

IPC_WebClient *PduRpcObj::mWebIpc = nullptr;
void PduRpcObj::rpc_export()
{
    mWebIpc = IPC_WebClient::bulid();
    jsonrpc_export("pduLogFun", pduLogFun);
    jsonrpc_export("pduSetCfg", pduSetCfg);
    jsonrpc_export("pduReadCfg", pduReadCfg);
    jsonrpc_export("pduSetData", pduSetData);
    jsonrpc_export("pduReadData", pduReadData);
    jsonrpc_export("pduSetString", pduSetString);
    jsonrpc_export("pduReadString", pduReadString);
}


void PduRpcObj::pduReadData(jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    double value = mWebIpc->getValue((uchar)its.at(0), (uchar)its.at(1),
                                 (uchar)its.at(2),(uchar)its.at(3), (uchar)its.at(4));
    //qDebug()<<"AAAAAAAA"<<its.at(0)<<its.at(1)<<its.at(2)<<its.at(3)<<its.at(4)
    //       << value << QTime::currentTime().toString("mm:ss zzz");
    responRpcData(r, its, value);
}

void PduRpcObj::pduSetData(jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 6);
    bool ret = mWebIpc->setting((uint)its.at(0), (uchar)its.at(1), (uchar)its.at(2),
                            (uchar)its.at(3), (uchar)its.at(4), (uint)its.at(5));
    responRpcData(r, its, ret?1:0);
}

void PduRpcObj::responRpcData(jsonrpc_request *r, const QVector<double> &its, double value)
{
    jsonrpc_return_success(r, "[%g,%g,%g,%g,%g,%g]" ,
                           its.at(0), its.at(1), its.at(2), its.at(3), its.at(4), value);
}

void PduRpcObj::responRpcString(jsonrpc_request *r, const QVector<double> &its, QString value)
{
    jsonrpc_return_success(r, "[%g,%g,%g,%g,%g,%Q]" ,
                           its.at(0), its.at(1), its.at(2), its.at(3), its.at(4),
                           value.toStdString().c_str());
}

void PduRpcObj::pduReadString(jsonrpc_request *r)
{
//    qDebug()<<__FUNCTION__<< "     "<<__LINE__;
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    QString value = mWebIpc->getString((uchar)its.at(0), (uchar)its.at(1),
                                 (uchar)its.at(4));
    responRpcString(r, its, value);
}

void PduRpcObj::pduSetString(jsonrpc_request *r)
{
    std::string value;
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    bool ret = JsonRpcObj::getString(r, 5, (char*)value.c_str());
    if(ret) ret = mWebIpc->setString((uint)its.at(0), (uchar)its.at(1),
                                     (uchar)its.at(4), (char*)value.c_str());
    responRpcData(r, its, ret?1:0);
}

void PduRpcObj::pduReadCfg(jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    double value = mWebIpc->getDevCfg((uchar)its.at(0), (uchar)its.at(1), (uchar)its.at(2));
    responRpcData(r, its, value);
}

void PduRpcObj::pduSetCfg(jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 6);
    bool ret = mWebIpc->setDevCfg((uint)its.at(0), (uchar)its.at(1),
                                  (uchar)its.at(2), (uint)its.at(5));
    responRpcData(r, its, ret?1:0);
}

void PduRpcObj::pduLogFun(jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    QString value = mWebIpc->log_fun((uchar)its.at(0), (uchar)its.at(1),
                                 (uint)its.at(2), (uchar)its.at(3)).toString();
    responRpcString(r, its, value);
}
