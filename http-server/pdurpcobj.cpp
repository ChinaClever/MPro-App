#include "pdurpcobj.h"

void PduRpcObj::rpc_export()
{
    jsonrpc_export("pduReadData", pduReadData);  // of RPC functions
    //jsonrpc_export("pduReadString", pduReadString);
    jsonrpc_export("pduSetData", pduSetData);
    //jsonrpc_export("pduSetString", pduSetString);
}


void PduRpcObj::pduReadData(struct jsonrpc_request *r)
{
    IPC_WebClient *ipc = IPC_WebClient::bulid();
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    double value = ipc->getValue((uchar)its.at(0), (uchar)its.at(1),
                                 (uchar)its.at(2),(uchar)its.at(3), (uchar)its.at(4));
    responRpc(r, its, value);
}

void PduRpcObj::pduSetData(struct jsonrpc_request *r)
{
    IPC_WebClient *ipc = IPC_WebClient::bulid();
    QVector<double> its = JsonRpcObj::getNumbers(r , 6);
    bool ret = ipc->setting((uchar)its.at(0), (uchar)its.at(1), (uchar)its.at(2),
                            (uchar)its.at(3), (uchar)its.at(4), (uint)its.at(5));
    responRpc(r, its, ret?1:0);
}

void PduRpcObj::responRpc(jsonrpc_request *r, const QVector<double> &its, double value)
{
    jsonrpc_return_success(r , "[%g,%g,%g,%g,%g,%g]" ,
                           its.at(0) , its.at(1) , its.at(2) , its.at(3) , its.at(4) , value);
}
