#include "pdurpcobj.h"

WebRpcObj *PduRpcObj::mObj = nullptr;
IPC_WebClient *PduRpcObj::mWebIpc = nullptr;
void PduRpcObj::rpc_export()
{
    mObj = new WebRpcObj();
    mWebIpc = IPC_WebClient::bulid();
}

char* PduRpcObj::pduReadData(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    double value = mWebIpc->getValue((uchar)its.at(0), (uchar)its.at(1),
                                     (uchar)its.at(2),(uchar)its.at(3), (uchar)its.at(4));
    return responRpcData(its, value);
}

char* PduRpcObj::pduSetData(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 6);
    bool ret = mWebIpc->setting((uint)its.at(0), (uchar)its.at(1), (uchar)its.at(2),
                                (uchar)its.at(3), (uchar)its.at(4), (uint)its.at(5));
    return responRpcData(its, ret?1:0);
}

char* PduRpcObj::responRpcData(const QVector<double> &its, double value)
{
    return mg_mprintf("[%g,%g,%g,%g,%g,%g]", its.at(0), its.at(1), its.at(2),
                      its.at(3), its.at(4), value);
}

char* PduRpcObj::responRpcString(const QVector<double> &its, const QString &value)
{
    return mg_mprintf("[%g,%g,%g,%g,%g,%Q]" , its.at(0), its.at(1), its.at(2),
                      its.at(3), its.at(4), value.toStdString().c_str());
}

char *PduRpcObj::pduReadParam(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);    
    QString value = mWebIpc->getCfg((uchar)its.at(0), (uchar)its.at(1), (uchar)its.at(2));
    return responRpcString(its, value);
}


char *PduRpcObj::pduSetParam(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    QVariant value = mObj->getString(r, 5);
    if(value.toString().isEmpty()) value = mObj->getNumber(r, 5);
    bool ret = mWebIpc->setCfg((uint)its.at(0), (uchar)its.at(1), (uchar)its.at(2), value);
    return responRpcData(its, ret?1:0);
}

char* PduRpcObj::pduLogFun(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    QString value = mWebIpc->log_fun((uchar)its.at(2), (uchar)its.at(3),
                                     (uint)its.at(0), (uchar)its.at(4)).toString();
    return responRpcString(its, value);
}





char* PduRpcObj::pduReadString(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    QString value = mWebIpc->getCfg((uchar)its.at(0), (uchar)its.at(1),
                                       (uchar)its.at(4));
    return responRpcString(its, value);
}

char* PduRpcObj::pduSetString(mg_str &r)
{
    bool ret = false;
    QVector<double> its = mObj->getNumbers(r, 5);
    QString str = mObj->getString(r, 5);
    qDebug()  << "CCCCCCCCCCCCC" << str;

    if(str.size()) ret = mWebIpc->setCfg((uint)its.at(0), (uchar)its.at(1), (uchar)its.at(4), str);
    return responRpcData(its, ret?1:0);
}

char* PduRpcObj::pduReadCfg(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    double value = mWebIpc->getCfg((uchar)its.at(0), (uchar)its.at(1), (uchar)its.at(2)).toDouble();
    return responRpcData(its, value);
}

char* PduRpcObj::pduSetCfg(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    QVariant value = mObj->getString(r, 5);
     qDebug()  << "AAAAAAAAAAA" << value;
    if(value.toString().isEmpty()) {
        value = mObj->getNumber(r, 5);
         qDebug()  << "BBBBBBBBBB" << value;
    }




    bool ret = mWebIpc->setCfg((uint)its.at(0), (uchar)its.at(1),
                                  (uchar)its.at(2), value);
    return responRpcData(its, ret?1:0);
}

