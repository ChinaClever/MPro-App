#include "devinfo.h"

void DevInfo::init()
{
    jsonrpc_export("pduGetInfo", pduGetInfo);  // of RPC functions
}

int DevInfo::getLineNum(int id)
{
    return cm::devData(id)->info.lineNum;
}

int DevInfo::getLoopNum(int id)
{
    return cm::devData(id)->info.loopNum;
}

int DevInfo::getOpNum(int id)
{
    return cm::devData(id)->info.outputNum;
}

void DevInfo::pduGetInfo(struct jsonrpc_request *r)
{
    double value = 0 , id = 0 , type = 0;
    QVector<double> its = JsonRpcObj::getNumbers(r , 2);
    id = its.at(0);
    type = its.at(1);
    switch((int)type)
    {
        case sDevSizeType::Line_Num: value = getLineNum((int)id); break;
        case sDevSizeType::Loop_Num: value = getLoopNum((int)id); break;
        case sDevSizeType::Output_Num: value = getOpNum((int)id); break;
        default:value = getLineNum((int)id); break;
    }
    jsonrpc_return_success(r , "[%g,%g,%g]" , its.at(0) , its.at(1) , value);
}


