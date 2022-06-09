#include "pdurpcobj.h"

IPC_WebClient *PduRpcObj::mWebIpc = nullptr;
void PduRpcObj::rpc_export()
{
    mWebIpc = IPC_WebClient::bulid();
    jsonrpc_export("pduSetData", pduSetData);
    jsonrpc_export("pduReadData", pduReadData);
    jsonrpc_export("pduSetString", pduSetString);
    jsonrpc_export("pduReadString", pduReadString);
}


void PduRpcObj::pduReadData(struct jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    double value = mWebIpc->getValue((uchar)its.at(0), (uchar)its.at(1),
                                 (uchar)its.at(2),(uchar)its.at(3), (uchar)its.at(4));
    responRpcData(r, its, value);
}

void PduRpcObj::pduSetData(struct jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r , 6);
    bool ret = mWebIpc->setting((uchar)its.at(0), (uchar)its.at(1), (uchar)its.at(2),
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

void PduRpcObj::pduReadString(struct jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    QString value = mWebIpc->getString((uchar)its.at(0), (uchar)its.at(1),
                                 (uchar)its.at(4));
    responRpcString(r, its, value);
}

void PduRpcObj::pduSetString(struct jsonrpc_request *r)
{
    std::string value;
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    bool ret = JsonRpcObj::getString(r, 5, (char*)value.c_str());
    if(ret) ret = mWebIpc->setString((uchar)its.at(0), (uchar)its.at(1),
                                     (uchar)its.at(4), (char*)value.c_str());
    responRpcData(r, its, ret?1:0);
}
/*
struct sStrItem{
    sStrItem():addr(0),rw(0){}
    uchar addr; // 地址
    uchar fc; // 10 输出位  11 UUT信息
    uchar id; // 0 表示统一设置
    uchar rw; // 0 读  1 写
    char str[NAME_SIZE];
};*/
