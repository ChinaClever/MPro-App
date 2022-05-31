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
//    return cm::devData(id)->info.outputNum;
    return 24;
}

void DevInfo::pduGetInfo(struct jsonrpc_request *r)
{
    double value = 0 , id = 0 , type = 0 , subtopic = 1;
    QVector<double> its = JsonRpcObj::getNumbers(r , 5);
    id = its.at(0);
    type = its.at(1);
    subtopic = its.at(3);
    if(subtopic == 0){
        switch((int)type)
        {
        case DType::Line: value = getLineNum((int)id); break;
        case DType::Loop: value = getLoopNum((int)id); break;
        case DType::Output: value = getOpNum((int)id); break;
        default:value = getLineNum((int)id); break;
        }
        jsonrpc_return_success(r , "[%g,%g,%g,%g,%g,%g]" , its.at(0) , its.at(1) ,its.at(2) , its.at(3),its.at(4), value);
    }
}


