#include "pdudevinfo.h"

void PduDevInfo::devInfoExport()
{
    jsonrpc_export("pduGetInfo", pduGetInfo);
}

double PduDevInfo::pduInfoNum(sDevData *dev, int type)
{
    double value = 0;
    switch(type) {
    case DType::Line: value = dev->info.lineNum; break;
    case DType::Loop: value = dev->info.loopNum; break;
    case DType::Output: value = dev->info.outputNum; break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    }
    return value;
}

/**
 *  0 地址
 *  1 功能码  1 设备配置数量 （ 输出位 回路数量）
 *           2
 *  2、
 *  3、
 *  4、
 */
void PduDevInfo::pduGetInfo(struct jsonrpc_request *r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r , 5);
    double id = its.at(0);
    double type = its.at(1);
    double subtopic = its.at(3);

    double value = 0;
    sDevData *dev = cm::devData(id);

    switch ((int)subtopic) {
    case DSub::Size: value = pduInfoNum(dev, (int)type); break;
        //    case DSub::Value: value =
    default: qDebug() << Q_FUNC_INFO << type; break;
    }

    responRpcData(r, its, value);
}


