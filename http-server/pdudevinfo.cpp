#include "pdudevinfo.h"

void PduDevInfo::devInfoExport()
{
    jsonrpc_export("pduGetInfo", pduGetInfo);
}

void PduDevInfo::pduGetInfo(struct jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r , 5);
    double id = its.at(0); double type = its.at(1);
    double subtopic = its.at(3); double value = 0;
    if(subtopic == DSub::Size) {
        sDevData *dev = cm::devData(id);
        switch((int)type) {
        case DType::Line: value = dev->info.lineNum; break;
        case DType::Loop: value = dev->info.loopNum; break;
        case DType::Output: value = dev->info.outputNum; break;
        default: qDebug() << Q_FUNC_INFO << type; break;
        }
        responRpcData(r, its, value);
  }
}


