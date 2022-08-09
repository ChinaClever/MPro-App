/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_rpc.h"

Web_Obj *Web_Rpc::mObj = nullptr;
Web_Rpc::Web_Rpc()
{
    mObj = new Web_Obj();
}

char* Web_Rpc::pduReadData(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    double value = mObj->getValue((uchar)its.at(0), (uchar)its.at(1),
                                  (uchar)its.at(2),(uchar)its.at(3), (uchar)its.at(4));
    return responRpcData(its, value);
}

char* Web_Rpc::pduSetData(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 6);
    bool ret = mObj->setting((uint)its.at(0), (uchar)its.at(1), (uchar)its.at(2),
                             (uchar)its.at(3), (uchar)its.at(4), (uint)its.at(5));
    return responRpcData(its, ret?1:0);
}

char* Web_Rpc::responRpcData(const QVector<double> &its, double value)
{
    return mg_mprintf("[%g,%g,%g,%g,%g,%g]", its.at(0), its.at(1), its.at(2),
                      its.at(3), its.at(4), value);
}

char* Web_Rpc::responRpcString(const QVector<double> &its, const QString &value)
{
    return mg_mprintf("[%g,%g,%g,%g,%g,%Q]" , its.at(0), its.at(1), its.at(2),
                      its.at(3), its.at(4), value.toStdString().c_str());
}

char *Web_Rpc::pduReadParam(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    QString value = mObj->getCfg((uchar)its.at(0), (uchar)its.at(1), (uchar)its.at(2));
    return responRpcString(its, value);
}

char *Web_Rpc::pduSetParam(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    QVariant value = mObj->getString(r, 5);
    if(value.toString().isEmpty()) value = mObj->getNumber(r, 5);
    bool ret = mObj->setCfg((uint)its.at(0), (uchar)its.at(1), (uchar)its.at(2), value);
    return responRpcData(its, ret?1:0);
}

char* Web_Rpc::pduLogFun(mg_str &r)
{
    QVector<double> its = mObj->getNumbers(r, 5);
    QString value = mObj->log_fun((uchar)its.at(2), (uchar)its.at(3),
                                  (uint)its.at(0), (uchar)its.at(4)).toString();
    return responRpcString(its, value);
}
