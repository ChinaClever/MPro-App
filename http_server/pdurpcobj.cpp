#include "pdurpcobj.h"

IPC_WebClient *PduRpcObj::mWebIpc = nullptr;
void PduRpcObj::rpc_export()
{
    mWebIpc = IPC_WebClient::bulid();
}

char* PduRpcObj::pduReadData(mg_str r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    double value = mWebIpc->getValue((uchar)its.at(0), (uchar)its.at(1),
                                 (uchar)its.at(2),(uchar)its.at(3), (uchar)its.at(4));
    //qDebug()<<"AAAAAAAA"<<its.at(0)<<its.at(1)<<its.at(2)<<its.at(3)<<its.at(4)
    //       << value << QTime::currentTime().toString("mm:ss zzz");
    return responRpcData(its, value);
}

char* PduRpcObj::pduSetData(mg_str r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 6);
    bool ret = mWebIpc->setting((uint)its.at(0), (uchar)its.at(1), (uchar)its.at(2),
                            (uchar)its.at(3), (uchar)its.at(4), (uint)its.at(5));
    return responRpcData(its, ret?1:0);
}

char* PduRpcObj::responRpcData(const QVector<double> &its, double value)
{
    return mg_mprintf("[%g,%g,%g,%g,%g,%g]" ,
                           its.at(0), its.at(1), its.at(2), its.at(3), its.at(4), value);
}

char* PduRpcObj::responRpcString(const QVector<double> &its, QString value)
{
    return mg_mprintf("[%g,%g,%g,%g,%g,%Q]" ,
                           its.at(0), its.at(1), its.at(2), its.at(3), its.at(4),
                           value.toStdString().c_str());
}

char* PduRpcObj::pduReadString(mg_str r)
{
//    qDebug()<<__FUNCTION__<< "     "<<__LINE__;
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    QString value = mWebIpc->getString((uchar)its.at(0), (uchar)its.at(1),
                                 (uchar)its.at(4));
    return responRpcString(its, value);
}

char* PduRpcObj::pduSetString(mg_str r)
{
    std::string value;
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    bool ret = JsonRpcObj::getString(r, 5, (char*)value.c_str());
    if(ret) ret = mWebIpc->setString((uint)its.at(0), (uchar)its.at(1),
                                     (uchar)its.at(4), (char*)value.c_str());
    return responRpcData(its, ret?1:0);
}

char* PduRpcObj::pduReadCfg(mg_str r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    double value = mWebIpc->getDevCfg((uchar)its.at(0), (uchar)its.at(1), (uchar)its.at(2));
    return responRpcData(its, value);
}

char* PduRpcObj::pduSetCfg(mg_str r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 6);
    bool ret = mWebIpc->setDevCfg((uint)its.at(0), (uchar)its.at(1),
                                  (uchar)its.at(2), (uint)its.at(5));
    return responRpcData(its, ret?1:0);
}

char* PduRpcObj::pduLogFun(mg_str r)
{
    QVector<double> its = JsonRpcObj::getNumbers(r, 5);
    QString value = mWebIpc->log_fun((uchar)its.at(0), (uchar)its.at(1),
                                 (uint)its.at(2), (uchar)its.at(3)).toString();
    return responRpcString(its, value);
}
